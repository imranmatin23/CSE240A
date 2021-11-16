//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include <math.h>
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
//       State Helper Functions       //
//------------------------------------//

int
increment_state(int state)
{
  if (state == ST) { return state; }
  return state + 1;
}

int
decrement_state(int state)
{
  if (state == SN) { return state; }
  return state - 1;
}

int 
get_new_state(int state, int outcome) 
{
  if (outcome == TAKEN) { return increment_state(state); } 
  return decrement_state(state);
}

uint8_t
get_prediction_from_state(int state)
{
  if (state == SN || state == WN) { return NOTTAKEN; } 
  return TAKEN;
}

//------------------------------------//
//             gshare                 //
//------------------------------------//

#define PHT_SIZE 1024
int PHT[PHT_SIZE];
uint32_t GHR;

int
combine_ghr_and_pc(uint32_t pc)
{
  int combined = GHR ^ pc;
  int mask = pow(2, ghistoryBits) - 1;
  int combined_and_masked = combined & mask;
  return combined_and_masked;
}

int
get_state_from_PHT(int address)
{
  int index = address % PHT_SIZE;
  int state = PHT[index];
  return state;
}

void 
get_new_state_in_PHT(int address, int new_state) {
    int index = address % PHT_SIZE;
    PHT[index] = new_state;
}

void
update_ghr(uint8_t outcome) {
  // shift GHR over by 1 bit
  GHR = GHR << 1;
  // insert new outcome
  GHR = GHR | outcome;
}

void 
init_gshare() 
{
  // Initialize the PHT.
  for(int i = 0; i < PHT_SIZE; ++i) {
     PHT[i] = WN;
  }
  // Initialize the GHR.
  GHR = 0;
}

uint8_t 
predict_gshare(uint32_t pc) 
{
  int combined = combine_ghr_and_pc(pc);
  int state = get_state_from_PHT(combined);
  int prediction = get_prediction_from_state(state);
  return prediction;
}

void
train_gshare(uint32_t pc, uint8_t outcome)
{
  int combined = combine_ghr_and_pc(pc);
  int current_state = get_state_from_PHT(combined);
  int new_state = get_new_state(current_state, outcome);
  get_new_state_in_PHT(combined, new_state);
  update_ghr(outcome);
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
      // init_tournament();
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
      // return predict_tournament(pc);
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
      // train_tournament(pc, outcome);
    case CUSTOM:
    default:
      break;
  }
}
