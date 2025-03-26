module;

import std;

export module timer;

export class TimeCtr
{
    std::thread* timerThrd;
    int miliseconds = 0;
    int seconds = 0;
    int minutes = 0;
public:
    bool wasReset = false;
    std::string GetMinutes()
    {
        return std::to_string(minutes);
    }
    std::string GetSeconds()
    {
        if (seconds > 9)
        {
            return std::to_string(seconds);
        }
        else
        {
            return "0" + std::to_string(seconds);
        }
    }
    std::string GetMiliseconds()
    {
        if (miliseconds > 9)
        {
            return std::to_string(miliseconds);
        }
        else
        {
            return "0" + std::to_string(miliseconds);
        }
    }
    void Start(bool& quit, bool& pause)
    {
        wasReset = true;
        timerThrd = new std::thread(&TimeCtr::Count, this, std::ref(quit), std::ref(pause));
    }
    void Count(bool& quit, bool& pause)
    {
        using namespace std::literals::chrono_literals;
        while (!quit)
        {
            if (!pause)
            {
                miliseconds++;
                if (miliseconds > 99)
                {
                    seconds++;
                    miliseconds = 0;
                    if (seconds > 59)
                    {
                        minutes++;
                        seconds = 0;
                    }
                }
            }
            std::this_thread::sleep_for(10ms);
        }
    }
    void SaveIfHigher(std::string file)
    {
        std::ifstream in;
        in.open(file);
        int bestTime;
        if (!in.is_open())
        {
            bestTime = 99999999999999999;
        }
        else
        {
            in >> bestTime;
        }

        in.close();
        int temp = miliseconds + (seconds * 100) + (minutes * 6000);

        if (temp < bestTime)
        {
            std::cout << "SAVING TIME - ";
            std::ofstream out;
            out.open(file);
            out << temp;
            out.close();
            std::cout << "done.\n";
        }
        return;
    }
    ~TimeCtr()
    {
        timerThrd->join();
        //std::cout << "TIMER DESTRUCTOR\n";
        delete(timerThrd);
    }
};
