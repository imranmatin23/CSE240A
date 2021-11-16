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

// Increments the state, and doesn't increment the state if it is the highest
// state already.
// 
int
increment_state(int state)
{
  if (state == ST) { return state; }
  return state + 1;
}

// Decrements the state, and doesn't decrement the state if it is the lowest
// state already.
// 
int
decrement_state(int state)
{
  if (state == SN) { return state; }
  return state - 1;
}

// Computes the new state based of the current state and the outcome.
// 
int 
get_new_state(int state, int outcome) 
{
  if (outcome == TAKEN) { return increment_state(state); } 
  return decrement_state(state);
}

// Computes the prediction from the state.
// 
uint8_t
get_prediction_from_state(int state)
{
  if (state == SN || state == WN) { return NOTTAKEN; } 
  return TAKEN;
}

//------------------------------------//
//             gshare                 //
//------------------------------------//

// NOTE: Does this change?
#define PATTERN_HISTORY_TABLE_SIZE 1024
uint32_t pattern_history_table[PATTERN_HISTORY_TABLE_SIZE];
uint32_t gshare_ghr;

// Returns the the pattern history table index by performing
// ( (gshare_ghr ^ pc) & ((2^ghistoryBits)-1) ) % PHT_SIZE. This ensures that only
// ghistoryBits are used. NOTE: Computes the index by modding by the
// PHT size, is this correct?
// 
uint32_t
get_pattern_history_table_index(uint32_t pc)
{
  uint32_t mask = pow(2, ghistoryBits) - 1;
  return ((gshare_ghr ^ pc) & mask) % PATTERN_HISTORY_TABLE_SIZE;
}

// Computes the index in the PHT for this PC, and updates the state at that
// index based on the true outcome. 
// 
void
train_gshare_PHT(uint32_t pc, uint8_t outcome)
{
  int index = get_pattern_history_table_index(pc);
  int current_state = pattern_history_table[index];
  int new_state = get_new_state(current_state, outcome);
  pattern_history_table[index] = new_state;
}

// Updates the gshare_ghr by shifting it by one bit then ORing it with the 
// true outcome. i.e. (gshare_ghr << 1) | outcome
void
train_gshare_ghr(uint8_t outcome) {
  gshare_ghr = (gshare_ghr << 1) | outcome;
}

// Initialize all entries of the Pattern History table to WN and the gshare_ghr
// to 0.
// 
void 
init_gshare() 
{
  for(int i = 0; i < PATTERN_HISTORY_TABLE_SIZE; ++i) {
     pattern_history_table[i] = WN;
  }
  gshare_ghr = 0;
}

// Gets the prediction for this PC. It first computes the index in the PHT 
// from the PC. It then computes the prediction from the state at that index.
// 
uint8_t 
predict_gshare(uint32_t pc) 
{
  int index = get_pattern_history_table_index(pc);
  int current_state = pattern_history_table[index];
  return get_prediction_from_state(current_state);
}

// Trains the gshare predictor by training the PHT and the GHR.
// 
void
train_gshare(uint32_t pc, uint8_t outcome)
{
  train_gshare_PHT(pc, outcome);
  train_gshare_ghr(outcome);
}

//------------------------------------//
//         tournament                 //
//------------------------------------//

#define CHOICE_PREDICT_SIZE 4096
#define GLOBAL_PREDICT_SIZE 4096
#define LOCAL_HISTORY_TABLE_SIZE 1024
#define LOCAL_PREDICT_SIZE 1024
uint32_t choice_predict[CHOICE_PREDICT_SIZE];
uint32_t global_predict[GLOBAL_PREDICT_SIZE];
uint32_t local_history_table[LOCAL_HISTORY_TABLE_SIZE];
uint32_t local_predict[LOCAL_PREDICT_SIZE];
uint32_t tournament_ghr;

uint32_t
get_tournament_ghr() {
  uint32_t mask = pow(2, ghistoryBits) - 1;
  uint32_t masked_ghr = tournament_ghr & mask;
  return masked_ghr;
}

uint32_t
get_local_history_table_index_from_pc(uint32_t pc) {
  uint32_t mask = pow(2, pcIndexBits) - 1;
  uint32_t masked_pc = pc & mask;
  return masked_pc % LOCAL_HISTORY_TABLE_SIZE;
}

uint32_t
get_local_predict_index_from_local_history_table(uint32_t index)
{
  uint32_t local_predict_index = local_history_table[index];
  uint32_t mask = pow(2, lhistoryBits) - 1;
  uint32_t masked_local_predict_index = local_predict_index & mask;
  return masked_local_predict_index % LOCAL_PREDICT_SIZE;
}

uint8_t 
predict_tournament_local(uint32_t pc)
{
  uint32_t local_history_table_index = get_local_history_table_index_from_pc(pc);
  uint32_t local_predict_index = get_local_predict_index_from_local_history_table(local_history_table_index);
  int current_state = local_predict[local_predict_index];
  return get_prediction_from_state(current_state);
}

uint8_t 
predict_tournament_global(uint32_t pc)
{
  uint32_t index = get_tournament_ghr();
  uint32_t modded_index = index % GLOBAL_PREDICT_SIZE;
  int current_state = global_predict[modded_index];
  return get_prediction_from_state(current_state);
}

uint8_t
predict_tournament_select_prediction(uint8_t local_prediction, uint8_t global_prediction)
{
  uint32_t index = get_tournament_ghr();
  uint32_t modded_index = index % CHOICE_PREDICT_SIZE;
  int current_state = choice_predict[modded_index];
  int predictor = get_prediction_from_state(current_state);
  if (predictor == 0) {
    return local_prediction;
  }
  return global_prediction;
}

void
train_tournament_global_predict(uint8_t outcome)
{
  uint32_t index = get_tournament_ghr();
  uint32_t modded_index = index % GLOBAL_PREDICT_SIZE;
  int current_state = global_predict[modded_index];
  int new_state = get_new_state(current_state, outcome);
  global_predict[modded_index] = new_state;
}

void
train_tournament_local_predict(uint32_t pc, uint8_t outcome)
{
  uint32_t local_history_table_index = get_local_history_table_index_from_pc(pc);
  uint32_t local_predict_index = get_local_predict_index_from_local_history_table(local_history_table_index);
  int current_state = local_predict[local_predict_index];
  int new_state = get_new_state(current_state, outcome);
  local_predict[local_predict_index] = new_state;
}

void
train_tournament_local_history_table(uint32_t pc, uint8_t outcome)
{
  uint32_t local_history_table_index = get_local_history_table_index_from_pc(pc);
  uint32_t current_state = local_history_table[local_history_table_index];
  int new_state = (current_state << 1) | outcome;
  local_history_table[local_history_table_index] = new_state;
}

void
train_tournament_ghr(uint8_t outcome)
{
  tournament_ghr = (tournament_ghr << 1) | outcome;
}

void
train_tournament_choice_predict(uint32_t pc, uint8_t outcome)
{
  int new_state;
  int local_prediction = predict_tournament_local(pc);
  int global_prediction = predict_tournament_global(pc);
  uint32_t index = get_tournament_ghr();
  uint32_t modded_index = index % CHOICE_PREDICT_SIZE;
  int current_state = choice_predict[modded_index];
  if (local_prediction == outcome) {
    new_state = decrement_state(current_state);
  } else {
    new_state = increment_state(current_state);
  }
  choice_predict[modded_index] = new_state;
}

void 
init_tournament() 
{
  for (int i = 0; i < CHOICE_PREDICT_SIZE; i++) {
     choice_predict[i] = WN;
  }
  for (int i = 0; i < GLOBAL_PREDICT_SIZE; i++) {
     global_predict[i] = WN;
  }
  for (int i = 0; i < LOCAL_HISTORY_TABLE_SIZE; i++) {
     local_history_table[i] = 0;
  }
  for (int i = 0; i < LOCAL_PREDICT_SIZE; i++) {
     local_predict[i] = WN;
  }
  tournament_ghr = 0;
}

uint8_t 
predict_tournament(uint32_t pc) 
{
  int local_prediction = predict_tournament_local(pc);
  int global_prediction = predict_tournament_global(pc);
  return predict_tournament_select_prediction(local_prediction, global_prediction);
}

void
train_tournament(uint32_t pc, uint8_t outcome)
{
  train_tournament_global_predict(outcome);
  train_tournament_choice_predict(pc, outcome);
  train_tournament_local_predict(pc, outcome);
  train_tournament_local_history_table(pc, outcome);
  train_tournament_ghr(outcome);
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
      init_tournament();
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
      return predict_tournament(pc);
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
      train_tournament(pc, outcome);
    case CUSTOM:
    default:
      break;
  }
}
