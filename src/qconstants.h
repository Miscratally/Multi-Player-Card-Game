#ifndef QCONSTANTS_H
#define QCONSTANTS_H

// namespace GameConstants
namespace GameConstants {
    // Window dimensions
    constexpr int WindowWidth = 450;
    constexpr int WindowHeight = 180;
    constexpr int WidgetWidth = 1280;
    constexpr int WidgetHeight = 720;

    // Player related constants
    constexpr int MinPlayerCount = 2;
    constexpr int MaxPlayerCount = 5;

    constexpr int SomePlayerCount = 10;

    constexpr int AvatorSize = 100;

    // Card related constants
    constexpr int NumberOfSuits = 4;
    constexpr int NumberOfCards = 13;

    // Timer related constant
    constexpr int TimeCountDown = 15;
    constexpr int TimerSize = 64;
    constexpr int TimerInterval = 1000;
}

#endif
