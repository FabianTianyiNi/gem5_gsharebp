#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_LRU_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_LRU_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct LRUIPVRPParams; //  LRUIPVRP

class LRUIPVRP : public BaseReplacementPolicy
{
  protected:
    /** LRU-specific implementation of replacement data. */
    struct LRUVRPReplData : ReplacementData
    {


        const uint64_t index;
      
    };

  public:
    /** Convenience typedef. */
    typedef LRUIPVRPParams Params;

    /**
     * Construct and initiliaze this replacement policy.
     */
    LRUIPVRP(const Params *p);

    /**
     * Destructor.
     */
    ~LRUIPVRP() {}

    /**
     * Invalidate replacement data to set it as the next probable victim.
     * Sets its last touch tick as the starting tick.
     *
     * @param replacement_data Replacement data to be invalidated.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
                                                              const override;

    /**
     * Touch an entry to update its replacement data.
     * Sets its last touch tick as the current tick.
     *
     * @param replacement_data Replacement data to be touched.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Reset replacement data. Used when an entry is inserted.
     * Sets its last touch tick as the current tick.
     *
     * @param replacement_data Replacement data to be reset.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Find replacement victim using LRU timestamps.
     *
     * @param candidates Replacement candidates, selected by indexing policy.
     * @return Replacement entry to be replaced.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                     override;

    /**
     * Instantiate a replacement data entry.
     *
     * @return A shared pointer to the new replacement data.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_LRU_RP_HH__