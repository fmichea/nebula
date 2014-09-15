#include "abstractmainwindow.hh"

static void* gbrun(void* data) {
    static_cast<nebula::frontends::AbstractMainWindow*>(data)->gbrun();
    return NULL;
}

namespace nebula { namespace frontends {

AbstractMainWindow::AbstractMainWindow()
    : display_ (nullptr), mmu_ (nullptr), gpu_ (nullptr), kb_ (nullptr), z80_ (nullptr)
{}

AbstractMainWindow::~AbstractMainWindow() {}

void AbstractMainWindow::execute(std::string romname) {
    // Allocate all the things
    this->mmu_ = new MMU();
    if (!this->mmu_->load_rom(romname))
        return;
    // ...
    this->kb_ = new Keyboard(this->mmu_);
    this->gpu_ = new GPU(this->display_, this->mmu_);
    this->z80_ = new Z80(this->mmu_, this->gpu_, this->kb_);
    // Initialize all the gameboy components and link them.

    // Now launch the GameBoy in a separate thread and let it run :)
    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&thread, &attr, ::gbrun, this);
    pthread_attr_destroy(&attr);

    // Now just handle UI events in a loop.
    this->run();

    // We are done if here, we just need to wait for the GameBoy to stop.
    pthread_cancel(thread);
    pthread_join(thread, NULL);

    // Delete all the things.
    delete this->z80_;
    delete this->gpu_;
    delete this->kb_;
    delete this->mmu_;
}

void AbstractMainWindow::gbrun() {
    this->z80_->run();
}

}} /* !namespace nebula::frontends */
