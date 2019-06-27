#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_LRU_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_LRU_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct LRUIPVRPParams; //  LRUIPVRP

class LRUIPVRP : public BaseReplacementPolicy
{

  private:

  typeof std::vector<bool> LRUList;

  /**
  set for the index of the nodes in the linked list
  **/
  const uint64_t numNodes;


  /**
  Holds the latest temporary list instance created by instantiateEntry().
  **/

  LRUList* listinstance;


  protected:
    /** LRU-specific implementation of replacement data. */
    struct LRUVRPReplData : ReplacementData
    {
        // /** Tick on which the entry was last touched. */
        // Tick lastTouchTick;

        // /**
        //  * Default constructor. Invalidate data.
        //  */
        // LRUVRPReplData() : lastTouchTick(0) {}

        const uint64_t index;

        /**
         * Shared list pointer. A list share between the nodes in the list, so
         * that accesses to a replacement data entry updates the LRUIPV bits of all other replacement data entries in its set
         */
        std::shared_ptr<LRUList> list;

        /**
        default constructor
        
        */
        LRUVRPReplData(const uint64_t index, std::shared_ptr<LRUList> list);
      
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