#include "mbed.h"
#include "uLCD_4DGL.h"


uLCD_4DGL uLCD(D1, D0, D2);

EventQueue queue(32 * EVENTS_EVENT_SIZE);

PwmOut PWM1(D6);
AnalogIn Ain(A0);


DigitalOut PinOut(D7);
InterruptIn PinIn(D8);

Thread t;
Thread thread;
Thread thread1;

int cnt = 0;

void uled_rise(int counter) {
   
   uLCD.color(WHITE);
   uLCD.text_width(4);
   uLCD.text_height(4);
   uLCD.locate(0,0);
   uLCD.printf("%4d\r\n", counter);
   uLCD.printf("Rise\n");
    
}
void uled_down(int counter) {
   uLCD.color(WHITE);
   uLCD.text_width(4);
   uLCD.text_height(4);
   uLCD.locate(0,0);
   uLCD.printf("%4d\r\n", counter);
   uLCD.printf("Down\n");
    
}

void ADC_record()
{
   float ADC;
   float sum = 0;
   
   while (true)
   {
    sum = 0;

      for (int i = 0; i < 50; i++) {
        sum += Ain;
        cnt++;
        ThisThread::sleep_for(1ms);    
      }
      
      sum /= 50;
      if (sum >= 0.5) PinOut = 1;
      else PinOut = 0;
      
   }
}

void Trig_Rise()  {
   queue.call(uled_rise, cnt);
}
void Trig_Down()  {
   queue.call(uled_down, cnt);
}

void led_breathing()
{
float width[20] = {0.0f, 0.2f, 0.4f, 0.6f , 0.8f, 1.0f, 1.0f, 1.0f, 1.0f , 1.0f, 
0.8f, 0.6f, 0.4f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    
    while (1) {
        PWM1.period_ms(5);
        for (int i = 0; i <= 19; i++) {
            PWM1.pulsewidth_ms(5 * width[i]);
            ThisThread::sleep_for(50ms);
        }    
    } 
}

int main() {
   thread.start(led_breathing);
   thread1.start(ADC_record);
   t.start(callback(&queue, &EventQueue::dispatch_forever));
   
   PinIn.rise(Trig_Rise);
   PinIn.fall(Trig_Down);
   while (1) {}

}