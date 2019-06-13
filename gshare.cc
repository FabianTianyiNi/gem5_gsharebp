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

    PHTCtrs.resize(PHTPredictorSize);

    for (int i = 0; i < PHTPredictorSize; ++i)
    {
        PHTCtrs[i].setBits(PHTCtrBits);
    }

    PHTThreshold = (ULL(1) << (PHTCtrBits - 1)) - 1;
    
}

// function of lookup()
bool
GshareBP::lookup(ThreadID tid, Addr branch_addr, void * &bp_history)
{
    unsigned PHTCtrsIdx = (branch_addr ^ globalHistory[tid]) & historyRegisterMask;
    assert(PHTCtrsIdx < PHTPredictorSize);

    //we can get the final results on the branch predictor coubters, but dont forget to uodate it afterwards
    bool final_Prediction = (PHTCtrs[PHTCtrsIdx].read() > PHTThreshold);

    // now update
    BPHistory *history = new BPHistory;
    history->finalPredictionResult = final_Prediction;
    history->globalHistory = globalHistory[tid];
    bp_history = <void*>(history);

    return final_Prediction;
    
}

// function of update() every time system makes a prediction, update the result to the currrent table
void
GshareBP::update(ThreadID tid, Addr branch_addr, bool taken,
                     void *bp_history, bool squashed)
{
    if(bp_history)
    {
        unsigned PHTCtrsIdx = (branch_addr ^ globalHistory[tid]) & historyRegisterMask;
        assert(PHTCtrsIdx < PHTPredictorSize);
        if(taken)
        {
            PHTCtrs[PHTCtrsIdx].increment();
        }
        else
        {
            PHTCtrs[PHTCtrsIdx].decrement();
        }

        // case: the result was mis-predicted
        if(squashed)
        {
            if(taken)
				globalHistory[tid] = (history->globalHistory << 1) | 1;
			else
				globalHistory[tid] = (history->globalHistory << 1);
			globalHistory[tid] &= historyRegisterMask;
        }
        else
		{
			delete history;
		}
    }
}


// function of btbUpdate()
void
GshareBP::btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history)
{
    globalHistory[tid] &= (historyRegisterMask & ~ULL(1));
}

// function of uncondBranch()
void
GshareBP::uncondBranch(ThreadID tid, Addr pc, void * &bp_history)
{
    // Create BPHistory and pass it back to be recorded.
    BPHistory *history = new BPHistory;
    history->globalHistory = globalHistory[tid];
    history->finalPredictionResult = true;
    bp_history = static_cast<void*>(history);
    updateGlobalHistReg(tid, true);
}

// function of squash()
void
GshareBP::squash(ThreadID tid, void *bp_history)
{
    BPHistory *history = static_cast<BPHistory*>(bp_history);
    globalHistory[tid] = history->globalHistory;
    delete history;
}

GshareBP*
GshareBPParams::create()
{
    return new GshareBP(this);
}   

void
GshareBP::updateGlobalHistReg(ThreadID tid, bool taken)
{
	//shift the register and insert the new value.
	globalHistory[tid] = taken ? (globalHistory[tid] << 1) | 1 :
								(globalHistory[tid] << 1);
	globalHistory[tid] &= historyRegisterMask;
}