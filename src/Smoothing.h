#ifndef _Smoothing_h_
#define _Smoothing_h_

class Smoothing30
{
private:
        static const int numReadings = 30;
        int readings[numReadings];   
        int readIndex = 0;              
        long total= 0;  
        int average = 0; 
public:
    Smoothing30();
    void add(int input);
    int getVal();
};

class Smoothing50
{
private:
        static const int numReadings = 50;
        int readings[numReadings];   
        int readIndex = 0;              
        long total= 0;  
        int average = 0; 
public:
    Smoothing50();
    void add(int input);
    int getVal();
};
class Smoothing200
{
private:
        static const int numReadings = 190;
        int readings[numReadings];   
        int readIndex = 0;              
        long total= 0;  
        int average = 0; 
public:
    Smoothing200();
    void add(int input);
    int getVal();
};
#endif  