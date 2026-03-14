enum Joystick {
    Left = 0x01,
    Right = 0x02,
    Up = 0x04,
    Down = 0x08,
    Button0 = 0x10,
    Button1 = 0x20,
	All = Left | Right | Up | Down | Button0 | Button1,
    Buttons = Button0 | Button1,
};

extern void InitInput();
extern uint8_t ReadButtons();