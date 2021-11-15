//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"

//
// Student Information
//
const char *studentName = "Imran Matin";
const char *studentID   = "A14473033";
const char *email       = "imatin@ucsd.edu";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

//------------------------------------//
//             constants              //
//------------------------------------//

// Define the size of the PHT, i.e. how many entries in the table.
#define PHT_SIZE 1024
// Declare the PHT.
int PHT[PHT_SIZE];

//------------------------------------//
//        Helper Functions            //
//------------------------------------//

void
print_PHT() 
{
  printf("PHT: ");
  for(int i = 0; i < PHT_SIZE; ++i) {
     printf("%d", PHT[i]);
  }
  printf("\n");
}

int
increment_state(int state)
{
  if (state == ST) {
    return state;
  }
  return state + 1;
}

int
decrement_state(int state)
{
  if (state == SN) {
    return state;
  }
  return state - 1;
}

int
get_prediction_from_state(int state)
{
  if (state == SN || state == WN) { return NOTTAKEN;} 
  else if (state == WT || state == ST) { return TAKEN; }
}

int 
update_state(int state, int outcome) 
{
  if (outcome == TAKEN) {
    return increment_state(state);
  } 
  else if (outcome == NOTTAKEN) 
  {
    return decrement_state(state);
  }
}


//------------------------------------//
//             gshare                 //
//------------------------------------//

// Initialize a PHT of size PHT_SIZE and set each 2-bit counter to WN.
// 
void 
init_gshare() 
{
  for(int i = 0; i < PHT_SIZE; ++i) {
     PHT[i] = WN;
  }
}

// Make a prediction using the gshare predictor.
// 1. Mod the PC and the PHT_SIZE.
// 2. Extract the state in the PHT for this PC.
// 3. Predict based on the state in the PHT.
// 
uint8_t 
predict_gshare(uint32_t pc) 
{
  int index = pc % PHT_SIZE;
  int state = PHT[index];
  int prediction = get_prediction_from_state(state);
  return prediction;
}

void
train_gshare(uint32_t pc, uint8_t outcome)
{
  int index = pc % PHT_SIZE;
  int current_state = PHT[index];
  int new_state = update_state(current_state, outcome);
  PHT[index] = new_state;
}

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void
init_predictor()
{
  switch (bpType) {
    case STATIC:
    case GSHARE:
      init_gshare();
    case TOURNAMENT:
    case CUSTOM:
    default:
      break;
  }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
      return predict_gshare(pc);
    case TOURNAMENT:
    case CUSTOM:
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void
train_predictor(uint32_t pc, uint8_t outcome)
{
  switch (bpType) {
    case STATIC:
    case GSHARE:
      train_gshare(pc, outcome);
    case TOURNAMENT:
    case CUSTOM:
    default:
      break;
  }
}
