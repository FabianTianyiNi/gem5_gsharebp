/*
 * Copyright (c) 2019, 2019 ARM Limited
 * All rights reserved
 * Author: Tianyi Jones-Ni
 * Arizona State University
 * Master of Computer Science, CSE 410/510 Computer Architecture II
 */

#include "cpu/pred/gshare.hh"

#include "base/bitfield.hh"
#include "base/intmath.hh"

GshareBP::GshareBP(const GshareBPParams *params)
    : BPredUnit(params),
      instShiftAmt(params->instShiftAmt),
      globalHistory(params->numThreads, 0), //initilize the global History registor to 0
      globalHistoryBits(ceilLog2(params->globalPredictorSize)),  //initilize the size of the global history register to be log2(localPredictorSize)
      globalPredictorSize(params->globalPredictorSize),
      PHTPredictorSize(params->PHTPredictorSize),
      PHTCtrBits(params->PHTCtrBits)
{
    if (!isPowerOf2(PHTPredictorSize)) {
        fatal("Invalid local predictor size!\n");
    }

    if (!isPowerOf2(globalPredictorSize)) {
        fatal("Invalid global predictor size!\n");
    }

    historyRegisterMask = mask(globalHistoryBits); //this is for the global history table mask

    localCtrs.resize(localPredictorSize);

    for (int i = 0; i < localPredictorSize; ++i)
    {
        localCtrs[i].setBits(PHTCtrBits);
    }

    localThreshold = (ULL(1) << (PHTCtrBits - 1)) - 1;
    
}

// function of lookup()
bool
GshareBP::lookup(ThreadID tid, Addr branch_addr, void * &bp_history)
{
    unsigned localCtrsIdx = (branch_addr ^ globalHistory[tid]) & historyRegisterMask;
    assert(localCtrsIdx < localPredictorSize);

    //we can get the final results on the branch predictor coubters, but dont forget to uodate it afterwards
    bool final_Prediction = (localCtrs[localCtrsIdx.read() > localThreshold]);

    // now update
    BPHistory *history = new BPHistory;
    history->finalPredictionResult = final_Prediction;
    history->globalHistoryReg = globalHistory[tid];
    bp_history = <void*>(history);

    return final_Prediction;
    
}

// function of update()
void
GshareBP::update(ThreadID tid, Addr branch_addr, bool taken,
                     void *bp_history, bool squashed)
{

}


// function of btbUpdate()
void
GshareBP::btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history)
{

}

// function of uncondBranch()
void
GshareBP::squash(ThreadID tid, Addr pc, void * &bp_history)
{

}

// function of uncondBranch()
void
GshareBP::squash(ThreadID tid, void *bp_history)
{

}

GshareBP*
GshareBPParams::create()
{
    return new GshareBP(this);
}   