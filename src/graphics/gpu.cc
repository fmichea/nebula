#include "gpu.hh"
#include <iostream>

GPU::GPU(nebula::frontends::AbstractDisplay* display, MMU* mmu)
    : mmu_ (mmu), wait_count (456), display_ (display)
{
    // Colors
    this->bgcolors_[0] = this->display_->getColor(255, 255, 255);
    this->bgcolors_[1] = this->display_->getColor(192, 192, 192);
    this->bgcolors_[2] = this->display_->getColor(96, 96, 96);
    this->bgcolors_[3] = this->display_->getColor(0, 0, 0);
}

GPU::~GPU()
{}

void GPU::do_cycle(uint8_t cycles)
{
    this->wait_count -= cycles;
    if (this->wait_count < 0)
    {
        switch (this->mmu_->STAT.mode.get())
        {
        case LCDC_MODE_0: // H-Blank
            if (this->mmu_->LY.get() == this->mmu_->LYC.get())
            {
                this->mmu_->STAT.coin_int.set(1);
                this->mmu_->IF.set(this->mmu_->IF.get() | INTERRUPT_STAT);
            }
            else
                this->mmu_->STAT.coin_int.set(0);
            if (this->mmu_->LCDC.LDE.get() && this->mmu_->LY.get() < 144)
                this->draw_line();
            this->wait_count += LCDC_MODE_0_CLKS;
            this->mmu_->LY.set(this->mmu_->LY.get() + 1);
            if (this->mmu_->LY.get() < 160)
                this->mmu_->STAT.mode.set(LCDC_MODE_2);
            else
                this->mmu_->STAT.mode.set(LCDC_MODE_1);
            if (this->mmu_->gb_type == GBType::CGB && this->mmu_->hdma_active)
                this->mmu_->do_hdma();
            break;
        case LCDC_MODE_1: // V-Blank
            this->mmu_->STAT.mode.set(LCDC_MODE_2);
            this->wait_count += LCDC_MODE_1_CLKS;
            this->timer_.adjust();
            this->mmu_->IF.set(this->mmu_->IF.get() | INTERRUPT_VBLANK);
            this->mmu_->LY.set(0);
            this->display_->commit();
            break;
        case LCDC_MODE_2:
            this->wait_count += LCDC_MODE_2_CLKS;
            this->mmu_->STAT.mode.set(LCDC_MODE_3);
            break;
        case LCDC_MODE_3:
            this->wait_count += LCDC_MODE_3_CLKS;
            this->mmu_->STAT.mode.set(LCDC_MODE_0);
            break;
        };
    }
}

static inline uint8_t correct_color(uint8_t color)
{
    return color * 255 / 31;
}

uint32_t GPU::bgp_to_color(uint8_t idx, uint8_t pal)
{
    if (this->mmu_->gb_type == GBType::GB)
        return this->bgcolors_[this->mmu_->BGP.C[idx].get()];
    else {
        uint16_t color = *(uint16_t*)(this->mmu_->cbgp_ + pal * 8 + idx * 2);
        return this->display_->getColor(
            correct_color(color & 0x1f),
            correct_color((color >> 5) & 0x1f),
            correct_color((color >> 10) & 0x1f));
    }
}

uint32_t GPU::obp_to_color(uint8_t idx, uint8_t pal)
{
    if (this->mmu_->gb_type == GBType::GB) {
        PaletteProxy* palette = &this->mmu_->OBP1;
        if (pal)
            palette = &this->mmu_->OBP2;
        return this->bgcolors_[palette->C[idx].get()];
    } else {
        // each palette is 8 bytes
        // each color is 2 bytes
        uint16_t color = *(uint16_t*)(this->mmu_->cobp_ + pal * 8 + idx * 2);
        return this->display_->getColor(
            correct_color(color & 0x1f),
            correct_color((color >> 5) & 0x1f),
            correct_color((color >> 10) & 0x1f));
    }
}


void GPU::draw_line()
{
    uint32_t line[WIDTH];
    int8_t   bg_priority[WIDTH];
    uint8_t  real_y = this->mmu_->LY.get() + this->mmu_->SCY.get();
    uint8_t  scx = this->mmu_->SCX.get();
    uint8_t  ds = this->mmu_->LCDC.BGTMDS.get();

    for (uint8_t x = 0; x < WIDTH; ++x) {
        bg_priority[x] = 0;
        line[x] = this->bgcolors_[0];
    }

    // Real background.
    if (this->mmu_->gb_type == GBType::CGB || this->mmu_->LCDC.BGD.get())
    {
        BGWTile bg_tile(this->mmu_, scx, real_y, ds);
        for (uint8_t x = 0; x < WIDTH; ++x)
        {
            uint8_t real_x = scx + x;
            if (real_x % 8 == 0)
                bg_tile = BGWTile(this->mmu_, real_x, real_y, ds);
            uint8_t col = bg_tile.color(real_x % 8);
            line[x] = this->bgp_to_color(col, bg_tile.attribute.palette);
            if (this->mmu_->gb_type == GBType::CGB && bg_tile.attribute.priority)
                bg_priority[x] = 1;
            if (col == 0 || !this->mmu_->LCDC.BGD.get())
                bg_priority[x] = -1;
        }
    }

    // Window on background.
    ds = this->mmu_->LCDC.WTMDS.get();
    if (this->mmu_->LCDC.WDE.get() && this->mmu_->WY.get() <= this->mmu_->LY.get())
    {
        int16_t tmp = this->mmu_->WX.get() - 7; // -7 is wired that way
        uint8_t delta_y = this->mmu_->LY.get() - this->mmu_->WY.get();
        for (int16_t x = tmp; x < WIDTH; ++x)
        {
            if (x < 0) continue;
            BGWTile win_tile = BGWTile(this->mmu_, x - tmp, delta_y, ds);
            uint8_t col = win_tile.color(x % 8);
            line[x] = this->bgp_to_color(col, win_tile.attribute.palette);
            if (this->mmu_->gb_type == GBType::CGB
                    && win_tile.attribute.priority)
                bg_priority[x] = 1;
            if (col == 0 || !this->mmu_->LCDC.BGD.get())
                bg_priority[x] = -1;
        }
    }

    // Draw objects.
    if (this->mmu_->LCDC.OBJSDE.get())
    {
        std::list<Sprite*> sprites = SpriteManager::get_sprites(
            this->mmu_, this->mmu_->LY.get());
        logging::debug("List size : %zu", sprites.size());
        for (auto it_ = sprites.begin(); it_ != sprites.end(); ++it_)
        {
            Sprite* sprite = *it_;
            for (int it = 0; it < 8; ++it)
            {
                uint8_t x = sprite->x_base() + it;
                if (x < WIDTH) {
                    uint8_t col = sprite->color(it);
                    if (col > 0 && (bg_priority[x] == -1
                            || (bg_priority[x] == 0 && sprite->above_bg)))
                        line[x] = obp_to_color(col, sprite->palette);
                }
            }
            delete sprite;
        }
    }

    // Drawing of the final background and objects.
    this->display_->lock();
    uint8_t y = this->mmu_->LY.get();
    for (uint8_t x = 0; x < WIDTH; x++) {
        this->display_->setPixel(x, y, line[x]);
    }
    this->display_->unlock();
}
