/*
  LRUIPVRP
  **/

#include "mem/cache/replacement_policies/lru_rp.hh"

#include <cassert>
#include <memory>

#include "params/LRURP.hh"






LRUIPVRP::LRUVRPReplData::LRUVRPReplData(
    const uint64_t index, std::shared_ptr<LRUList> list)
    : index(index), list(list)
{
}


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
    // Update last touch timestamp
    //std::static_pointer_cast<LRUReplData>(replacement_data)->lastTouchTick = curTick();

    std::shared_ptr<LRUVRPReplData> LRUIPV_replacement_data = std::static_pointer_cast<LRUVRPReplData>(replacement_data);
    LRUList* list = LRUIPV_replacement_data->list.get();

    uint64_t list_index = LRUIPV_replacement_data->index;

    do{

    } while(list_index >= 0 || list_index < 16);
}

void
LRUIPVRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Set last touch timestamp
    //std::static_pointer_cast<LRUReplData>(replacement_data)->lastTouchTick = curTick();
    //std::shared_ptr<LRUVRPReplData> LRUIPV_replacement_data = std::static_pointer_cast<LRUVRPReplData>(replacement_data);

    // THE NEW INCOMING BLOCK WILL BE INSERTED INTO THE POSITION 13

}

ReplaceableEntry*
LRUIPVRP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    // Visit all candidates to find victim
    ReplaceableEntry* victim = candidates[13];
    for (const auto& candidate : candidates) {
        // Update victim entry if necessary
        if (std::static_pointer_cast<LRUReplData>(candidate->replacementData)->lastTouchTick < std::static_pointer_cast<LRUReplData>(victim->replacementData)->lastTouchTick) {
            victim = candidate;
        }
    }

    return victim;
}

std::shared_ptr<ReplacementData>
LRUIPVRP::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new LRUReplData());
}

LRURP*
LRUIPVRPParams::create()
{
    return new LRUIPVRP(this);
}
