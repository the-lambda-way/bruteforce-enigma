#pragma once

#include <chrono>
#include <cstdio>


class Stopwatch
{
public:
     void start ()
     {
          begin = std::chrono::steady_clock::now();
          is_running = true;
     }

     void stop ()
     {
          end = std::chrono::steady_clock::now();
          face = end - begin;
          is_running = false;
     }

     void click ()
     {
          if (is_running)     stop();
          else                start();
     }

     void read () const
     {
          std::printf("Elapsed time: %.2f seconds\n", face.count());
     }


private:
     std::chrono::time_point<std::chrono::steady_clock> begin;
     std::chrono::time_point<std::chrono::steady_clock> end;
     std::chrono::duration<double> face;
     bool is_running = false;

}; // class Stopwatch