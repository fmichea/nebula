#include "gpu.hh"
#include <iostream>

GPU::GPU(MMU& mmu)
    : mmu_ (mmu), wait_count (456), display_ (nullptr)
{
#if _TARGET == 0
    this->display_ = new SDLDisplay("nebula");
#endif

    // Colors
    this->colors_[0] = this->display_->getColor(255, 255, 255);
    this->colors_[1] = this->display_->getColor(192, 192, 192);
    this->colors_[2] = this->display_->getColor(96, 96, 96);
    this->colors_[3] = this->display_->getColor(0, 0, 0);
}

GPU::~GPU()
{
    delete this->display_;
}

void GPU::do_cycle(uint8_t cycles)
{
    this->wait_count -= cycles;
    if (this->wait_count < 0)
    {
        switch (this->mmu_.STAT.mode.get())
        {
        case LCDC_MODE_0: // H-Blank
            if (this->mmu_.LY.get() == this->mmu_.LYC.get())
            {
                this->mmu_.STAT.coin_int.set(1);
                this->mmu_.IF.set(this->mmu_.IF.get() | 0x2);
            }
            else
                this->mmu_.STAT.coin_int.set(0);
            if (this->mmu_.LCDC.LDE.get() && this->mmu_.LY.get() < 144)
                this->draw_line();
            this->wait_count += 207;
            this->mmu_.LY.set(this->mmu_.LY.get() + 1);
            if (this->mmu_.LY.get() < 160)
                this->mmu_.STAT.mode.set(LCDC_MODE_2);
            else
                this->mmu_.STAT.mode.set(LCDC_MODE_1);
            break;
        case LCDC_MODE_1: // V-Blank
            this->mmu_.STAT.mode.set(LCDC_MODE_2);
            this->wait_count += 4560; // Cycles for V-Blank.
            this->timer_.adjust();
            this->mmu_.IF.set(this->mmu_.IF.get() | 0x1);
            this->mmu_.LY.set(0);
            this->display_->commit();
            break;
        case LCDC_MODE_2:
            this->wait_count += 80;
            this->mmu_.STAT.mode.set(LCDC_MODE_3);
            break;
        case LCDC_MODE_3:
            this->wait_count += 171;
            this->mmu_.STAT.mode.set(LCDC_MODE_0);
            break;
        };
    }
}

void GPU::draw_line()
{
    uint8_t bkg[WIDTH];
    uint8_t real_y = this->mmu_.LY.get() + this->mmu_.SCY.get();
    uint8_t scx = this->mmu_.SCX.get();
    uint8_t ds = this->mmu_.LCDC.BGTMDS.get();

    this->display_->lock();
    memset(bkg, 0, WIDTH);

    // Real background.
    if (this->mmu_.LCDC.BGD.get())
    {
        //printf("Color: ");
        BGWTile bg_tile(this->mmu_, scx, real_y, ds);
        for (uint8_t x = 0; x < WIDTH; ++x)
        {
            uint8_t real_x = scx + x;
            if (real_x % 8 == 0)
                bg_tile = BGWTile(this->mmu_, real_x, real_y, ds);
            bkg[x] = this->mmu_.BGP.C[bg_tile.color(real_x % 8)].get();
        }
    }

    // Window on background.
    ds = this->mmu_.LCDC.WTMDS.get();
    if (this->mmu_.LCDC.WDE.get() && this->mmu_.WY.get() <= this->mmu_.LY.get())
    {
        int16_t tmp = this->mmu_.WX.get() - 7;
        uint8_t delta_y = this->mmu_.LY.get() - this->mmu_.WY.get();
        for (int16_t x = tmp; x < WIDTH; ++x)
        {
            if (x < 0) continue;
            BGWTile win_tile = BGWTile(this->mmu_, x - tmp, delta_y, ds);
            bkg[x] = this->mmu_.BGP.C[win_tile.color(x % 8)].get();
        }
    }

    // Draw objects.
    uint8_t objs[WIDTH];
    for (uint8_t x = 0; x < WIDTH; ++x)
        objs[x] = 0xff;
    if (this->mmu_.LCDC.OBJSDE.get())
    {
        std::list<Sprite*> sprites = SpriteManager::get_sprites(
            this->mmu_, this->mmu_.LY.get()
        );
        logging::debug("List size : %zu", sprites.size());
        for (auto it_ = sprites.begin(); it_ != sprites.end(); ++it_)
        {
            Sprite* sprite = *it_;
            for (int it = 0; it < 8; ++it)
            {
                uint8_t x = sprite->x_base() + it;
                if (x < WIDTH && sprite->is_displayed(it, bkg[x]))
                    objs[x] = sprite->color(it);
            }
            delete sprite;
        }
    }

    // Drawing of the final background and objects.
    uint8_t y = this->mmu_.LY.get();
    for (uint8_t x = 0; x < WIDTH; x++)
    {
        int res = 0;
        if (objs[x] == 0xff && !this->mmu_.LCDC.BGD.get())
            res = this->display_->setPixel(x, y, this->colors_[0]);
        else if (objs[x] != 0xff)
            res = this->display_->setPixel(x, y, this->colors_[objs[x]]);
        else
            res = this->display_->setPixel(x, y, this->colors_[bkg[x]]);
        if (res < 0)
            logging::error("Display::setPixel failed!");
    }
    this->display_->unlock();
}
