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
//             constants              //
//------------------------------------//

// Define the size of the PHT, i.e. how many entries in the table.
#define PHT_SIZE 1024
// Declare the PHT.
int PHT[PHT_SIZE];
// Declare the GHR
uint32_t GHR;

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

void
print_ghr()
{
  int mask = pow(2, ghistoryBits) - 1;
  printf("GHR: %lu\n", (unsigned long) (GHR & mask));
}

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
get_prediction_from_state(int state)
{
  if (state == SN || state == WN) { return NOTTAKEN; } 
  return TAKEN;
}

int 
update_state(int state, int outcome) 
{
  if (outcome == TAKEN) { return increment_state(state); } 
  return decrement_state(state);
}

//------------------------------------//
//             gshare                 //
//------------------------------------//

// Combines the GHR and PC using the XOR operation.
// 1. Executes GHR XOR PC
// 2. Uses the a mask of (2^ghistoryBits)-1 to select the desired combined bits.
// 
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
update_state_in_PHT(int address, int new_state) {
    int index = address % PHT_SIZE;
    PHT[index] = new_state;
}

// Updates the GHR with the latest branch outcome.
// 1. Shifts the GHR left by 1 bit and fills the LSB with 0.
// 2. Executes GHR OR outcome to insert the outcome into the LSB of GHR.
void
update_ghr(uint8_t outcome) {
  // shift GHR over by 1 bit
  GHR = GHR << 1;
  // insert new outcome
  GHR = GHR | outcome;
}

// 1. Initialize a PHT of size PHT_SIZE and set each 2-bit counter to WN.
// 
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

// Make a prediction using the gshare predictor.
// 1. Mod the PC and the PHT_SIZE.
// 2. Extract the state in the PHT for this PC.
// 3. Predict based on the state in the PHT.
// 
uint8_t 
predict_gshare(uint32_t pc) 
{
  int combined = combine_ghr_and_pc(pc);
  int state = get_state_from_PHT(combined);
  int prediction = get_prediction_from_state(state);
  return prediction;
}

// 1. Execute GHR XOR PC.
// 2. Mod the result and the PHT_SIZE.
// 3. Extract the state in the PHT for this PC.
// 4. Update the state in the PHT based on the outcome.
void
train_gshare(uint32_t pc, uint8_t outcome)
{
  int combined = combine_ghr_and_pc(pc);
  int current_state = get_state_from_PHT(combined);
  int new_state = update_state(current_state, outcome);
  update_state_in_PHT(combined, new_state);
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
