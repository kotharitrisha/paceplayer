#include "mbed.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX); // tx, rx
AnalogIn a1(p18); //

Serial xbee1(p9, p10);
DigitalOut rst1(p21);

Timer timer;

int s_count;
float t;
int flag;
int curr_pace, prev_pace;



void program_select()
{
    int time;
    int program;
    int num;

    xbee1.printf("P\n");
    wait(0.5);
    flag  = 1;
    //ee
    pc.printf("Welcome to Program Select\r\nChoose from the following speeds\r\n");
    pc.printf("Slow (80 bps): 1 tap\r\nMedium (120 bps): 2 taps\r\nFast (150 bps): 3 taps\r\n");

    while(a1.read()<0.5)
        wait(0.2);
    timer.reset();
    timer.reset();
    flag =1;
    num = 0;

    while (timer.read() < 2) {
        wait(0.05);
        if (a1.read()>0.5) {
            if (flag == 0) {
                num++;
                flag = 1;
            }
        }

        if (a1.read()<0.1)
            flag = 0;
    }

    program = num + 1;
    if (program > 3)
        program = 3;

    pc.printf("You have selected program %d\r\n", program);

    xbee1.printf("%d\n", program);
    timer.reset();

}


int detect_speed()
{
    int num = 0;
    float step = 0.00;
    float prev_step = -10.00;
    int pace;

    flag = 1;

    timer.reset();
    timer.start();

    while (num<4) {

        /*  Reset timer if above 15 seconds*/
        if (timer.read() > 15) {
            while (a1.read()<0.5)
                wait(0.2);
            timer.reset();
            timer.start();
            flag = 1;
            num = 0;
        }

        wait(0.05);
        if (a1.read()>0.5) {            // Step sensed

            if (flag == 0) {
                if (num>0)
                    prev_step = step;
                step = timer.read();

                /* Switch to Program Select*/
                if (step - prev_step < 0.4) {  // Quick 2-step signals Program Select
                    program_select();
                    return 0;
                }

                num++;
                flag++;
                s_count++;
                pc.printf("%f\r\n",step);
            }
        }

        if (a1.read()<0.01) {   // Clear flag
            flag = 0;
        }

    }

    pace = 8/(step/60);
    pc.printf("Pace:%d bpm\r\n",pace,s_count);

    return pace;
}


int main()
{

    rst1 = 0;
    wait_ms(1);
    rst1 = 1;
    s_count= 0;
    

    /* Step once to begin*/
    while (a1.read()<0.5)
        wait(0.2);              // Won't send anything until sensor is pressed at least once!
    timer.reset();
    timer.start();


    while(1) {
        prev_pace = curr_pace;
        curr_pace = detect_speed();
        s_count+=10;
        if (abs(curr_pace-prev_pace) <= 15)
            xbee1.printf("%d %d\r\n", curr_pace, s_count);

    }

    return 0;
}

