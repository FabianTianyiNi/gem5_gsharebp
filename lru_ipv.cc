/*
  LRUIPVRP
  **/

#include "mem/cache/replacement_policies/lru_rp.hh"

#include <cassert>
#include <memory>

#include "params/LRURP.hh"




LRUIPVRP::LRUIPVRP(const Params *p)
    : BaseReplacementPolicy(p)
{
    ipv = {0,0,1,0,3,0,1,2,1,0,5,1,0,0,1,11,13};
}

void
LRUIPVRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    
}

void    
LRUIPVRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    //THIS FUNCTION IS FOR A HIT AND HOW THE MODEL ADJUSTS AFTERWARDS
    for (i = ipv[replacement_data->index];i < replacement_data->index; i++)
    {
        recency_stack[i]++;
    }

    replacement_data->index = ipv[replacement_data->index];
}

void
LRUIPVRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // THIS FUNCTION IS FOR EVERY TIME YOU HAVE A NEW INSERTION AND HOW THE SYSTEM WILL ADJUST TO IT
    for (int i =13; i < 16; i++)
    {
        recency_stack[i]++;
    }

    replacement_data->index = 13;
    recency_stack[13] = replacement_data;
}

ReplaceableEntry*
LRUIPVRP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    for (int i = 0; i < condidates.size(); i++)
    {
        //candidates[i]->index = recency_stack[i];
        recency_stack[i] = candidates[i]->index;
    }
    // Visit all candidates to find victim
    ReplaceableEntry* victim = candidates[13];
    for (const auto& candidate : candidates) {
        
        }
    }

    return victim;
}

std::shared_ptr<ReplacementData>
LRUIPVRP::instantiateEntry()
{
    std::vector<int> recency_stack = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600};
    return std::shared_ptr<ReplacementData>(new LRUVRPReplData());
}

LRURP*
LRUIPVRPParams::create()
{
    return new LRUIPVRP(this);
}
