// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6
#define xCenter       509
#define yCenter       516
#define thhold        300
#define pauseX        300

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      64

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second
int yPin = A1;
int xPin = A0;
int buttonPin = 2;

int buttonState = 0;
int player = 0;
int point0 = 0;
int point1 = 0;
int prexled;

//초기화
int matrixled[64] = 
{2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2};

int xled = 0;
int yled = 0;



void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pixels.begin(); // This initializes the NeoPixel library.
  randomSeed(analogRead(2)); 
  Serial.begin(9600); 
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);    
  //activate pull-up resistor on the push-button pin
  pinMode(buttonPin, INPUT_PULLUP);   
  zero();
}

void loop() {
  zero(); //초기 깜빡이는 LED 호출
}

void move_minusx() {
	if (xled<7) {      
	  xled=xled+1; 
	  prexled = xled-1;
	  while (matrixled[xled]!=2) {
		if (xled >7) {
		  xled=prexled;
		  break;
		}
		xled++;       
	  }
	}
}

void move_plusx() {
    if (xled > 0) {
      xled=xled-1;
      prexled = xled+1;
      while (matrixled[xled]!=2) {
        if (xled < 0) {
          xled=prexled;
          break;
        }
        xled--;       
      }      
    } 
}


void zero() {
  //Serial.print("player = ");
  //Serial.println(player);
  for(float i=30;i>=0;i=i-0.5) {
	buttonState = digitalRead(buttonPin);
	if(analogRead(xPin) < xCenter-thhold) {
    pixels.setPixelColor(xled, pixels.Color(0,0,0));
    pixels.show();
		move_minusx();
    //Serial.println("< xCenter-thhold");
    delay(pauseX);
    i=0;
	}
	else if (analogRead(xPin) > xCenter+thhold) {
    pixels.setPixelColor(xled, pixels.Color(0,0,0));
    pixels.show();		
		move_plusx();
   //Serial.println("> xCenter+thhold");
    delay(pauseX);
    i=0;
	}
	if (buttonState == 0) {
    pixels.setPixelColor(xled, pixels.Color(0,0,0));
    pixels.show();  
		selectled();
    delay(pauseX); 
    //Serial.println("Button Clicked");   
		player=player^1;    
    i=0;
		return;
	}	
    if (player) {
      pixels.setPixelColor(xled, pixels.Color(0,i,i/3)); // Moderately bright green color.
    }
    else {
      pixels.setPixelColor(xled, pixels.Color(i,0,i/3)); // Moderately bright green color.
    }
      pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(100); // Delay for a period of time (in milliseconds).
  for(int i=0;i<=30;i=i+1) {
	buttonState = digitalRead(buttonPin); 
	if(analogRead(xPin) < xCenter-thhold) {
    pixels.setPixelColor(xled, pixels.Color(0,0,0));
    pixels.show();		
		move_minusx();
    //Serial.println("Decrease minusX");
    delay(pauseX);
    i=31;
	}
	else if (analogRead(xPin) > xCenter+thhold) {
    pixels.setPixelColor(xled, pixels.Color(0,0,0));
    pixels.show();		
	  move_plusx();
    //Serial.println("Decrease pluxX");
    delay(pauseX);
    i=31;
	}
	if (buttonState == 0) {
    pixels.setPixelColor(xled, pixels.Color(0,0,0));
    pixels.show();  
		selectled();    
    delay(pauseX);
		player=player^1;
    i=31;
		return;
	}	
    if (player) {
      pixels.setPixelColor(xled, pixels.Color(0,i,i/3)); // Moderately bright green color.
    }
    else {
      pixels.setPixelColor(xled, pixels.Color(i,0,i/3)); // Moderately bright green color.
    }
      pixels.show(); // This sends the updated pixel color to the hardware.
  }  
  delay(50); // Delay for a period of time (in milliseconds).
  pixels.setPixelColor(xled, pixels.Color(0,0,0)); 
}

void empty_pixels() {
	pixels.begin();
	for(int i=0;i<NUMPIXELS;i++){
		pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
		pixels.show(); // This sends the updated pixel color to the hardware.
		delay(1); // Delay for a period of time (in milliseconds).
	}
}

void selectled() {
  int r, g;
  if (player) {
    r = 0;
    g = 20;
  }
  else {
    r = 20;
    g = 0;
  }
  yled = xled;
  while (yled < 64) {
    yled = yled + 8; 
    if (matrixled[yled] < 2) { //초기 값2가 아니면 값이 있다고 판단(player0 = 0, player1 = 1)
      break;
    }
    if (matrixled[yled] == 2) { //초기 값이면 
      pixels.setPixelColor(yled, pixels.Color(r,g,10)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(100); // Delay for a period of time (in milliseconds).
      pixels.setPixelColor(yled, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }  
  }
  yled = yled - 8;
  matrixled[yled] = player;
  pixels.setPixelColor(yled, pixels.Color(r,g,10)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.  
  if (yled<8) {
    xled=0;
    while (matrixled[xled]!=2 & xled<7) {
      xled++;
    }
  }
  
  // x축 계산, 무조건 좌측부터 8개까지 해당 player 확인 (x/8로 확인)
  point0 = 0;
  int x = yled/8;
  for (int i=0;i<8;i++) {
    if (matrixled[x*8+i] == player) point0++;
    else point0 = 0;
    // 4점이면 해당 라인 깜빡이기
    if (point0 == 4) {
      while(1) {
        if (digitalRead(buttonPin)==0) {
          reset_value();
          return;      
        }         
        for(int j=4;j>0;j--) {
          pixels.setPixelColor(x*8+i-j+1, pixels.Color(r,g,10)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.       
          delay(100);      
        }
        delay(100);
        for(int j=4;j>0;j--) {
          pixels.setPixelColor(x*8+i-j+1, pixels.Color(0,0,0)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.   
          delay(100);            
        }
        delay(10);                 
      }
      break;
    }        
  }

  // y축 계산, 무조건 상단부터 8개까지 해당 player 확인 (x%8로 확인)
  point0 = 0;
  int y = yled%8;
  for (int i=0;i<8;i++) {
    if (matrixled[i*8+y] == player) point0++;
    else point0 = 0;
    // 4점이면 해당 라인 깜빡이기
    if (point0 == 4) {
      while(1) {
        if (digitalRead(buttonPin)==0) {
          reset_value();
          return;     
        }         
        for(int j=4;j>0;j--) {
          pixels.setPixelColor(i*8+y-(8*(j-1)), pixels.Color(r,g,10)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.       
          delay(100);      
        }
        delay(100);
        for(int j=4;j>0;j--) {
          pixels.setPixelColor(i*8+y-(8*(j-1)), pixels.Color(0,0,0)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.   
          delay(100);            
        }
        delay(10);                 
      }
      break;
    }        
  }

 // 점수 대각선 \축 확인 후 계산 ---> raw = yled/8, col = yled%8
  point0 = 0;
  int raw = yled/8;
  int col = yled%8;
  int i=0;
  if (raw < col) { i = col-raw; col=col-raw; raw=0; }
  else { i=(raw-col)*8; raw=raw-col; col=0; }             
  for (i;i<64 & raw<8 & col<8;) {
    if (matrixled[i] == player) point0++;
    else point0 = 0;	
    // 4점이면 해당 라인 깜빡이기
    if (point0 == 4) {
      while(1) {
        if (digitalRead(buttonPin)==0) {
          reset_value();
          return;      
        }         
        for(int j=4;j>0;j--) {            
          pixels.setPixelColor(raw*8+col, pixels.Color(r,g,10)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.           
          delay(100);      
          raw--;
          col--;
        }
        delay(100);
        raw=raw+4;
        col=col+4;
        for(int j=4;j>0;j--) {          
          pixels.setPixelColor(raw*8+col, pixels.Color(0,0,0)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.   
          delay(100); 
          raw--;
          col--;           
        }
        raw=raw+4;
        col=col+4;        
        delay(10);                 
      }
    }
	raw++;
  col++;
	i=raw*8+col;	
  }  
  

 // 점수 대각선 /축 확인 후 계산 ---> raw = yled/8, col = yled%8
  point0 = 0;
  raw = yled/8;
  col = yled%8;
  i=0;
  if (raw >= 7-col) { raw=raw-(7-col); col=7; i=raw*8+col; }
  else { i=raw+col; col=raw+col; raw=0; }         
  for (i;i<64 & raw<8 & col>=0;) {
    if (matrixled[i] == player) point0++;
    else point0 = 0;  
    // 4점이면 해당 라인 깜빡이기
    if (point0 == 4) {
      while(1) {      
        if (digitalRead(buttonPin)==0) {
          reset_value();
          return;     
        }        
        for(int j=4;j>0;j--) {            
          pixels.setPixelColor(raw*8+col, pixels.Color(r,g,10)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.           
          delay(100);      
          raw--;
          col++;
        }
        delay(100);
        raw=raw+4;
        col=col-4;
        for(int j=4;j>0;j--) {
          pixels.setPixelColor(raw*8+col, pixels.Color(0,0,0)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.   
          delay(100); 
          raw--;
          col++;           
        }
        raw=raw+4;
        col=col-4;        
        delay(10);                 
      }
    }
  raw++;
  col--;
  i=raw*8+col;  
  }  
}

void reset_value() {
          buttonState = 0;
          //player = 0;
          point0 = 0;
          point1 = 0;
    
          for(int i=0;i<64;i++) {
            matrixled[i] = 2;
          }      
          xled = 0;
          yled = 0;
          empty_pixels();
          delay(500); 
}

//알고리즘
//https://gist.github.com/beancrab/5883364
//http://chilles.tistory.com/15
//http://mathworld.wolfram.com/Connect-Four.html
