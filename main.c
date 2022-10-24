#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define BUTTON1 1

uint8_t dl = 0;
bool working = false;
uint8_t effect = 0;
uint8_t effect_data = 0;

// Intializing arrays with HEX values, which represent states of leds on the circut

const char light_to_centre[] = {0x81, 0x42, 0x24,
  0x18, 0x24, 0x42};

const uint8_t shadow[] = {0xFE, 0xFD, 0xFB, 0xF7,
  0xEF, 0xDF, 0xBF, 0x7F};

const uint8_t jhonson_counter[] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 
  0x7F, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00};



unsigned char is_button_pressed();


// Function to change effect`s state and the effect itself
void run_effect (bool state) 
{
  if (state == 1)
  {
      switch (effect)
      {
      case 0:
        PORTD = light_to_centre[effect_data];
        if (++effect_data > 5) effect_data = 0;
        break;

      case 1:
        PORTD = shadow[effect_data];
        if(++effect_data > 7) effect_data = 0;
        break;

      case 2:
        PORTD = jhonson_counter[effect_data];
        if(++effect_data > 15) 
        effect_data = 0;
        break;
      }
  }

}

// Simple function to detect for how long button was holded.
void button_scan ()
{
  uint16_t timer = 0;

  while (is_button_pressed()) 
  {
    timer++;
    _delay_ms(25);
  }

  if (timer >= 8 && working == false)
  {
    if (effect > 1) 
    {
      effect = 0;
    } else 
    {
      effect++;
    }
  }

  if (timer < 8 && timer != 0) {
    PORTD = 0x00;
    effect_data = 0;
    working = !working;
  }

}

// Function to check whether the button was pressed 
unsigned char is_button_pressed() {
  if(!(PINB & (1 << BUTTON1))) 
  {
    _delay_ms(25);
    if (!(PINB & (1<<BUTTON1))) return 1;
  }
  return 0;
}

int main(void)
{
  // Setting inputs and outputs
  DDRD = 0xFF;
  DDRB = 0x00;

  for (;;) 
  {
    if (++dl > 20)
    {
      dl = 0;
      run_effect(working);
    }
    button_scan();
    _delay_ms(10);
  }


  return 0;
