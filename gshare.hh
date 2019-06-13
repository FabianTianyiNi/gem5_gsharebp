/*
 * Copyright (c) 2019, 2019 ARM Limited
 * All rights reserved
 * Author: Tianyi Jones-Ni
 * Arizona State University
 * Master of Computer Science, CSE 410/510 Computer Architecture II
 */

#ifndef __CPU_PRED_TOURNAMENT_PRED_HH__
#define __CPU_PRED_TOURNAMENT_PRED_HH__

#include <vector>

#include "cpu/pred/sat_counter.hh"
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/GshareBP.hh"

class GshareBP: public BPredUnit
{
    public:

        GshareBP(const GshareBPParams *params);

        bool lookup(ThreadID tid, Addr branch_addr, void * &bp_history);

        void uncondBranch(ThreadID tid, Addr pc, void * &bp_history);

        void btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history);

        void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget);

        void squash(ThreadID tid, void *bp_history);

    private:

         void updateGlobalHistReg(ThreadID tid, bool taken);

         struct BPHistory {
            unsigned globalHistory;
            /*
                the final taken/not-taken prediction
                true: predict taken
                false: predict not-taken
            */
            bool finalPredictionResult;
        };

        unsigned instShiftAmt;

        unsigned globalHistory;

        unsigned historyRegisterMask;

        unsigned globalHistoryBits;

        unsigned globalPredictorSize

        std::vector<SatCounter> PHTCtrs;

        unsigned PHTPredictorSize;

        unsigned PHTCtrBits;

        //unsigned localPredictorMask;

        unsigned PHTThreshold;

};

#endif // __CPU_PRED_GSHARE_PRED_HH__
