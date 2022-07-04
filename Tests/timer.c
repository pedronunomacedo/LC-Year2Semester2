/* um monte de defines com valores do enunciado */
#define BIT(n) (1<<n)

enum l3223_time_units {
    l3223_microsecond,
    l3223_millisecond,
    l3223_second
};

int hook_id = 0;
bool one_shot = false;
uint8_t ih_timer;
void timer_ih() {

}

int test_timer_one_shot(int timer, int interval, enum l3223_time_units unit) {
    uint8_t ctrl = 0x0;

    switch(timer) {
        case 0: ctrl = ctrl;
        case 1: ctrl |= BIT(6);
        case 2: ctrl |= BIT(7) | BIT(6);
    }

    if (unit == "13223_microsecond") {
        ctrl = ctrl;
    } else if (unit == "13223_millisecond") {
        ctrl |= BIT(0);
    }
    else if (unit == "13223_second") {
        ctrl |= BIT(1);
    }

    if (sys_outb(CTRL_REG, ctrl) != 0) {
        printf("ERROR: sys_outb(CTRL_REG) failed!\n");
        return 1;
    }

    uint8_t lsb = (uint8_t) interval;
    uint8_t msb = (uint8_t) (interval >> 8);

    int timer_reg = 0;
    switch (timer) {
        case 0: timer_reg = 0x20;
        case 1: timer_reg = 0x21;
        case 2: timer_reg = 0x22;
    }

    if (sys_outb(timer_reg, lsb) != 0) {
        printf("ERROR: sys_outb(CTRL_REG, lsb) failed!\n");
        return 1;
    }
    else if (sys_outb(timer_reg, msb) != 0) {
        printf("ERROR: sys_outb(CTRL_REG, msb) failed!\n");
        return 1;
    }

    bool end = false;
    ih_timer = timer;
    irq_set = BIT(hook_id);
    if (irq_setpolicy(TIMER_IRQ, IRQ_REENABLE, &hook_id)) return 1;
    
    while ( !end ) {
    // Driver receive e outras cenas do stor
        if (msg.m_notify.interrupts & irq_set) {
            timer_ih();
            if (one_shot) {
                end = true;
                pp_print_alarm(); // Função fornecida pelo professor
            }
        }
    }

    return r;
}