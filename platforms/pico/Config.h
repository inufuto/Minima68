#pragma once

class Config {
public:
    static constexpr auto SystemClock = 157500; // (kHz)
    class Gpio {
    public:
        static constexpr auto Video = 15;
        static constexpr auto Sound = 28;

        static constexpr auto UpButton = 8;
        static constexpr auto LeftButton = 9;
        static constexpr auto RightButton = 20;
        static constexpr auto DownButton = 21;
        static constexpr auto Button1 = 22;
        static constexpr auto Button0 = 26;

        static constexpr auto SdMiso = 16;
        static constexpr auto SdCs = 17;
        static constexpr auto SdSck = 18;
        static constexpr auto SdMosi = 19;
    };
};