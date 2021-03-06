#pragma once


#define TIME_QUANT                  128
#define TIME_SECONDS_PER_QUANT      30
#define TIME_UNPACK_GAME_SECONDS    (__int64)TIME_SECONDS_PER_QUANT / (double)TIME_QUANT
                // TIME_UNPACK_GAME_SECONDS = 0.234375
                // 30 game seconds per one time quant (128ms)   [128 * 0.234375 = 30]
                // seconds = game_time * TIME_UNPACK_GAME_SECONDS
#define TIME_PACK_GAME_SECONDS      (__int64)TIME_QUANT / (double)TIME_SECONDS_PER_QUANT
                // game_time += seconds * TIME_PACK_GAME_SECONDS

struct GameTime
{
    GameTime():
        value(0) { ; }
    GameTime(__int64 val) :
        value(val) { ; }
    GameTime(int seconds, int minutes, int hours = 0, int days = 0, int weeks = 0, int months = 0, int years = 0)
    {
        auto converted =
            (
                seconds
                + 60 * minutes
                + 3600 * hours
                + 86400 * days
                + 604800 * weeks
                + (__int64)2419200 * months
                + (__int64)29030400 * years
            )
            * TIME_PACK_GAME_SECONDS;

        this->value = (__int64)converted;
    }

    __int64 GetSeconds() const { return (__int64)(this->value * TIME_UNPACK_GAME_SECONDS); }
    __int64 GetMinutes() const { return this->GetSeconds() / 60i64; }
    __int64 GetHours() const { return this->GetMinutes() / 60i64; }
    int GetDays() const { return (int)(this->GetHours() / 24); }
    int GetWeeks() const { return this->GetDays() / 7; }
    int GetMonths() const { return this->GetWeeks() / 4; }
    int GetYears() const { return this->GetMonths() / 12; }

    int GetSecondsFraction() const { return this->GetSeconds() % 60; }
    int GetMinutesFraction() const { return (this->GetSeconds() / 60) % 60; }
    int GetHoursOfDay() const { return (this->GetSeconds() / 3600) % 24; }
    int GetDaysOfWeek() const { return this->GetDays() % 7; }
    int GetDaysOfMonth() const { return this->GetDays() % 28; }
    int GetWeeksOfMonth() const { return this->GetWeeks() % 4; }
    int GetMonthsOfYear() const { return this->GetMonths() % 12; }

    void AddMinutes(int minutes) { this->value += (__int64)(60 * minutes * TIME_PACK_GAME_SECONDS); }
    void SubtractHours(int hours) { this->value -= (__int64)(60 * 60 * hours * TIME_PACK_GAME_SECONDS); }

    void Reset() { this->value = 0; }
    bool Valid() const { return this->value > 0; }

    GameTime  operator +(GameTime &rhs)  { return GameTime(this->value + rhs.value); }
    GameTime  operator -(GameTime &rhs)  { return GameTime(this->value - rhs.value); }
    GameTime &operator +=(GameTime &rhs) { this->value += rhs.value; return *this; }

    bool operator >(GameTime &rhs) { return this->value > rhs.value; }
    bool operator >=(GameTime &rhs) { return this->value >= rhs.value; }
    bool operator <(GameTime &rhs) { return this->value < rhs.value; }
    bool operator <=(GameTime &rhs) { return this->value <= rhs.value; }

   explicit operator bool() { return this->Valid(); } // unsafe bool was casuing many problems

   operator int() { return this->value; } // cast operator conversion require

    static GameTime FromSeconds(int seconds) { return GameTime(seconds, 0, 0, 0, 0, 0, 0); }
    static GameTime FromMinutes(int minutes) { return GameTime(0, minutes, 0, 0, 0, 0, 0); }
    static GameTime FromHours(int hours)     { return GameTime(0, 0, hours, 0, 0, 0, 0); }
    static GameTime FromDays(int days)       { return GameTime(0, 0, 0, days, 0, 0, 0); }
    static GameTime FromYears(int years)     { return GameTime(0, 0, 0, 0, 0, 0, years); }

    __int64 value;
};


/*   61 */
#pragma pack(push, 1)
struct Timer
{
    static Timer *Create() { return new Timer; }

    Timer() :
        bReady(false), bPaused(false)
    {
        bTackGameTime = 0;
        uStartTime = 0;
        uStopTime = 0;
        uGameTimeStart = 0;
        field_18 = 0;
        uTimeElapsed = 0;
        dt_in_some_format = 0;
        uTotalGameTimeElapsed = 0;
    }

    void Initialize();
    unsigned __int64 Time();
    void Update();
    void Pause();
    void Resume();
    void TrackGameTime();
    void StopGameTime();

    unsigned int bReady;
    unsigned int bPaused;
    int bTackGameTime;
    unsigned int uStartTime;
    unsigned int uStopTime;
    int uGameTimeStart;
    int field_18;
    unsigned int uTimeElapsed;
    int dt_in_some_format;
    unsigned int uTotalGameTimeElapsed;

    static const unsigned int Minute = 2 * TIME_QUANT;
    static const unsigned int Hour = 60 * Minute;
    static const unsigned int Day = 24 * Hour;
    static const unsigned int Week = 7 * Day;
    static const unsigned int Month = 4 * Week;
    static const unsigned int Year = 12 * Month;
};

#pragma pack(pop)



extern Timer *pMiscTimer;
extern Timer *pEventTimer;