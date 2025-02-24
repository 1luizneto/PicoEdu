#include "inc/notes.h"

//int parar[] = {pause};

//MARIO 

int melody_mario[] = {
   
  E5,8, E5,8, pause,8, E5,8, pause,8, C5,8, E5,8, //1
  G5,4, pause,4, G4,8, pause,4, 
  C5,4, G4,8, pause,4, E4,4, // 3
  A4,4, B4,4, As4,8, A4,4,
  G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
  pause,8, E5,4,C5,8, D5,8, B4,4,
  C5,4, G4,8, pause,4, E4,4, // repeats from 3
  A4,4, B4,4, As4,8, A4,4,
  G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
  pause,8, E5,4,C5,8, D5,8, B4,4,

  
  pause,4, G5,8, Fs5,8, F5,8, Ds5,4, E5,8,//7
  pause,8, Gs4,8, A4,8, C4,8, pause,8, A4,8, C5,8, D5,8,
  pause,4, Ds5,4, pause,8, D5,4,
  C5,2, pause,2,

  pause,4, G5,8, Fs5,8, F5,8, Ds5,4, E5,8,//repeats from 7
  pause,8, Gs4,8, A4,8, C4,8, pause,8, A4,8, C5,8, D5,8,
  pause,4, Ds5,4, pause,8, D5,4,
  C5,2, pause,2,

  C5,8, C5,4, C5,8, pause,8, C5,8, D5,4,//11
  E5,8, C5,4, A4,8, G4,2,

  C5,8, C5,4, C5,8, pause,8, C5,8, D5,8, E5,8,//13
  pause,1, 
  C5,8, C5,4, C5,8, pause,8, C5,8, D5,4,
  E5,8, C5,4, A4,8, G4,2,
  E5,8, E5,8, pause,8, E5,8, pause,8, C5,8, E5,4,
  G5,4, pause,4, G4,4, pause,4, 
  C5,4, G4,8, pause,4, E4,4, // 19
  
  A4,4, B4,4, As4,8, A4,4,
  G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
  pause,8, E5,4, C5,8, D5,8, B4,4,

  C5,4, G4,8, pause,4, E4,4, // repeats from 19
  A4,4, B4,4, As4,8, A4,4,
  G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
  pause,8, E5,4, C5,8, D5,8, B4,4,

  E5,8, C5,4, G4,8, pause,4, Gs4,4,//23
  A4,8, F5,4, F5,8, A4,2,
  D5,8, A5,8, A5,8, A5,8, G5,8, F5,8,
  
  E5,8, C5,4, A4,8, G4,2, //26
  E5,8, C5,4, G4,8, pause,4, Gs4,4,
  A4,8, F5,4, F5,8, A4,2,
  B4,8, F5,4, F5,8, F5,8, E5,8, D5,8,
  C5,8, E4,4, E4,8, C4,2,

  E5,8, C5,4, G4,8, pause,4, Gs4,4,//repeats from 23
  A4,8, F5,4, F5,8, A4,2,
  D5,8, A5,8, A5,8, A5,8, G5,8, F5,8,
  
  E5,8, C5,4, A4,8, G4,2, //26
  E5,8, C5,4, G4,8, pause,4, Gs4,4,
  A4,8, F5,4, F5,8, A4,2,
  B4,8, F5,4, F5,8, F5,8, E5,8, D5,8,
  C5,8, E4,4, E4,8, C4,2,
  C5,8, C5,4, C5,8, pause,8, C5,8, D5,8, E5,8,
  pause,1,

  C5,8, C5,4, C5,8, pause,8, C5,8, D5,4, //33
  E5,8, C5,4, A4,8, G4,2,
  E5,8, E5,8, pause,8, E5,8, pause,8, C5,8, E5,4,
  G5,4, pause,4, G4,4, pause,4, 
  E5,8, C5,4, G4,8, pause,4, Gs4,4,
  A4,8, F5,4, F5,8, A4,2,
  D5,8, A5,8, A5,8, A5,8, G5,8, F5,8,
  
  E5,8, C5,4, A4,8, G4,2, //40
  E5,8, C5,4, G4,8, pause,4, Gs4,4,
  A4,8, F5,4, F5,8, A4,2,
  B4,8, F5,4, F5,8, F5,8, E5,8, D5,8,
  C5,8, E4,4, E4,8, C4,2,
  
  //game over sound
  C5,4, G4,4, E4,4, //45
  A4,8, B4,8, A4,8, Gs4,8, As4,8, Gs4,8,
  G4,8, D4,8, E4,2,  

};

// int melodyB[] = {
   
//   E5,8, E5,8, pause,8, E5,8, pause,8, C5,8, E5,8, //1
//   G5,4, pause,4, G4,8, pause,4, 
//   C5,4, G4,8, pause,4, E4,4, // 3
//   A4,4, B4,4, As4,8, A4,4,
//   G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
//   pause,8, E5,4,C5,8, D5,8, B4,4,
//   C5,4, G4,8, pause,4, E4,4, // repeats from 3
//   A4,4, B4,4, As4,8, A4,4,
//   G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
//   pause,8, E5,4,C5,8, D5,8, B4,4,

  
//   pause,4, G5,8, Fs5,8, F5,8, Ds5,4, E5,8,//7
//   pause,8, Gs4,8, A4,8, C4,8, pause,8, A4,8, C5,8, D5,8,
//   pause,4, Ds5,4, pause,8, D5,4,
//   C5,2, pause,2,

//   pause,4, G5,8, Fs5,8, F5,8, Ds5,4, E5,8,//repeats from 7
//   pause,8, Gs4,8, A4,8, C4,8, pause,8, A4,8, C5,8, D5,8,
//   pause,4, Ds5,4, pause,8, D5,4,
//   C5,2, pause,2,

//   C5,8, C5,4, C5,8, pause,8, C5,8, D5,4,//11
//   E5,8, C5,4, A4,8, G4,2,

//   C5,8, C5,4, C5,8, pause,8, C5,8, D5,8, E5,8,//13
//   pause,1, 
//   C5,8, C5,4, C5,8, pause,8, C5,8, D5,4,
//   E5,8, C5,4, A4,8, G4,2,
//   E5,8, E5,8, pause,8, E5,8, pause,8, C5,8, E5,4,
//   G5,4, pause,4, G4,4, pause,4, 
//   C5,4, G4,8, pause,4, E4,4, // 19
  
//   A4,4, B4,4, As4,8, A4,4,
//   G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
//   pause,8, E5,4, C5,8, D5,8, B4,4,

//   C5,4, G4,8, pause,4, E4,4, // repeats from 19
//   A4,4, B4,4, As4,8, A4,4,
//   G4,8, E5,8, G5,8, A5,4, F5,8, G5,8,
//   pause,8, E5,4, C5,8, D5,8, B4,4,

//   E5,8, C5,4, G4,8, pause,4, Gs4,4,//23
//   A4,8, F5,4, F5,8, A4,2,
//   D5,8, A5,8, A5,8, A5,8, G5,8, F5,8,
  
//   E5,8, C5,4, A4,8, G4,2, //26
//   E5,8, C5,4, G4,8, pause,4, Gs4,4,
//   A4,8, F5,4, F5,8, A4,2,
//   B4,8, F5,4, F5,8, F5,8, E5,8, D5,8,
//   C5,8, E4,4, E4,8, C4,2,

//   E5,8, C5,4, G4,8, pause,4, Gs4,4,//repeats from 23
//   A4,8, F5,4, F5,8, A4,2,
//   D5,8, A5,8, A5,8, A5,8, G5,8, F5,8,
  
//   E5,8, C5,4, A4,8, G4,2, //26
//   E5,8, C5,4, G4,8, pause,4, Gs4,4,
//   A4,8, F5,4, F5,8, A4,2,
//   B4,8, F5,4, F5,8, F5,8, E5,8, D5,8,
//   C5,8, E4,4, E4,8, C4,2,
//   C5,8, C5,4, C5,8, pause,8, C5,8, D5,8, E5,8,
//   pause,1,

//   C5,8, C5,4, C5,8, pause,8, C5,8, D5,4, //33
//   E5,8, C5,4, A4,8, G4,2,
//   E5,8, E5,8, pause,8, E5,8, pause,8, C5,8, E5,4,
//   G5,4, pause,4, G4,4, pause,4, 
//   E5,8, C5,4, G4,8, pause,4, Gs4,4,
//   A4,8, F5,4, F5,8, A4,2,
//   D5,8, A5,8, A5,8, A5,8, G5,8, F5,8,
  
//   E5,8, C5,4, A4,8, G4,2, //40
//   E5,8, C5,4, G4,8, pause,4, Gs4,4,
//   A4,8, F5,4, F5,8, A4,2,
//   B4,8, F5,4, F5,8, F5,8, E5,8, D5,8,
//   C5,8, E4,4, E4,8, C4,2,
  
//   //game over sound
//   C5,4, G4,4, E4,4, //45
//   A4,8, B4,8, A4,8, Gs4,8, As4,8, Gs4,8,
//   G4,8, D4,8, E4,-2,  

// };

//NATAL

// int melodyA[] = {
  
//   C5,4, //1
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8,
//   E5,4, C5,4, C5,4,
//   A5,4, A5,8, As5,8, A5,8, G5,8,
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,

//   F5,2, C5,4, //8 
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8,
//   E5,4, C5,4, C5,4,
//   A5,4, A5,8, As5,8, A5,8, G5,8,
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4,

//   F5,4, F5,4, F5,4,//17
//   E5,2, E5,4,
//   F5,4, E5,4, D5,4,
//   C5,2, A5,4,
//   As5,4, A5,4, G5,4,
//   C6,4, C5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4, 
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
  
//   G5,4, G5,8, A5,8, G5,8, F5,8, //27
//   E5,4, C5,4, C5,4,
//   A5,4, A5,8, As5,8, A5,8, G5,8,
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4,
//   F5,4, F5,4, F5,4,
//   E5,2, E5,4,
//   F5,4, E5,4, D5,4,
  
//   C5,2, A5,4,//36
//   As5,4, A5,4, G5,4,
//   C6,4, C5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4, 
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8, 
//   E5,4, C5,4, C5,4,
  
//   A5,4, A5,8, As5,8, A5,8, G5,8,//45
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4,
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8,
//   E5,4, C5,4, C5,4,
  
//   A5,4, A5,8, As5,8, A5,8, G5,8, //53
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, pause,4
// };

// int melodyB[] = {

//   C5,4, //1
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8,
//   E5,4, C5,4, C5,4,
//   A5,4, A5,8, As5,8, A5,8, G5,8,
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,

//   F5,2, C5,4, //8 
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8,
//   E5,4, C5,4, C5,4,
//   A5,4, A5,8, As5,8, A5,8, G5,8,
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4,

//   F5,4, F5,4, F5,4,//17
//   E5,2, E5,4,
//   F5,4, E5,4, D5,4,
//   C5,2, A5,4,
//   As5,4, A5,4, G5,4,
//   C6,4, C5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4, 
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
  
//   G5,4, G5,8, A5,8, G5,8, F5,8, //27
//   E5,4, C5,4, C5,4,
//   A5,4, A5,8, As5,8, A5,8, G5,8,
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4,
//   F5,4, F5,4, F5,4,
//   E5,2, E5,4,
//   F5,4, E5,4, D5,4,
  
//   C5,2, A5,4,//36
//   As5,4, A5,4, G5,4,
//   C6,4, C5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4, 
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8, 
//   E5,4, C5,4, C5,4,
  
//   A5,4, A5,8, As5,8, A5,8, G5,8,//45
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, C5,4,
//   F5,4, F5,8, G5,8, F5,8, E5,8,
//   D5,4, D5,4, D5,4,
//   G5,4, G5,8, A5,8, G5,8, F5,8,
//   E5,4, C5,4, C5,4,
  
//   A5,4, A5,8, As5,8, A5,8, G5,8, //53
//   F5,4, D5,4, C5,8, C5,8,
//   D5,4, G5,4, E5,4,
//   F5,2, pause,4
// };

//ASA BRANCA

//const int melodyA_length = sizeof(melodyA) / sizeof(melodyA[0]);

int melodyA[] = {
    G4,8, A4,8, B4,4, D5,4, D5,4, B4,4, C5,4, C5,2, G4,8, A4,4, B4,4, D5,4, D5,2,
    C5,8, B4,8, pause,8, G4,8, G4,4, A4,4, B4,8, D5,8, pause,8, D5,8, C5,4, B4,4,
    G4,8, C5,8, pause,8, C5,8, B4,4, A4,4, A4,8, B4,8, pause,8, B4,8, A4,4, G4,4,
    G4,2, pause,8, G4,8, G4,4, A4,4, B4,8, D5,8, pause,8, D5,8, C5,4, B4,4, G4,8,
    C5,8, pause,8, C5,8, B4,4, A4,4, A4,8, B4,8, pause,8, B4,8, A4,4, G4,4, G4,8,
    F5,8, D5,8, E5,8, C5,8, D5,8, B4,8, C5,8, A4,8, B4,8, G4,8, A4,8, G4,8, E4,8,
    G4,4, G4,8, F5,8, D5,8, E5,8, C5,8, D5,8, B4,8, C5,8, A4,8, B4,8, G4,8, A4,8,
    G4,8, E4,8, G4,2, pause,2
};

//const int melodyB_length = sizeof(melodyB) / sizeof(melodyB[0]);

int melodyB[] = {
    G4,8, A4,8, B4,4, D5,4, D5,4, B4,4, C5,4, C5,2, G4,8, A4,4, B4,4, D5,4, D5,2,
    C5,8, B4,8, pause,8, G4,8, G4,4, A4,4, B4,8, D5,8, pause,8, D5,8, C5,4, B4,4,
    G4,8, C5,8, pause,8, C5,8, B4,4, A4,4, A4,8, B4,8, pause,8, B4,8, A4,4, G4,4,
    G4,2, pause,8, G4,8, G4,4, A4,4, B4,8, D5,8, pause,8, D5,8, C5,4, B4,4, G4,8,
    C5,8, pause,8, C5,8, B4,4, A4,4, A4,8, B4,8, pause,8, B4,8, A4,4, G4,4, G4,8,
    F5,8, D5,8, E5,8, C5,8, D5,8, B4,8, C5,8, A4,8, B4,8, G4,8, A4,8, G4,8, E4,8,
    G4,4, G4,8, F5,8, D5,8, E5,8, C5,8, D5,8, B4,8, C5,8, A4,8, B4,8, G4,8, A4,8,
    G4,8, E4,8, G4,2, pause,2
};


// char* melody_char[] = {
//     "G4", "A4", "B4", "D5", "D5", "B4", "C5", "C5", "G4", "A4", "B4", "D5", "D5", "C5",
//     "B4", "--", "G4", "G4", "A4", "B4", "D5", "--", "D5", "C5", "B4", "G4", "C5", "--", "C5", "B4", "A4",
//     "A4", "B4", "--", "B4", "A4", "G4", "G4", "--", "G4", "G4", "A4", "B4", "D5", "--", "D5", "C5", "B4",
//     "G4", "C5", "--", "C5", "B4", "A4", "A4", "B4", "--", "B4", "A4", "G4", "G4", "F5", "D5", "E5", "C5", "D5", "B4",
//     "C5", "A4", "B4", "G4", "A4", "G4", "E4", "G4", "G4", "F5", "D5", "E5", "C5", "D5", "B4",
//     "C5", "A4", "B4", "G4", "A4", "G4", "E4", "G4", "G4", "--"
// };



