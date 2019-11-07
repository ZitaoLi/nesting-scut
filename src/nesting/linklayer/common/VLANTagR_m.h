//
// Generated file, do not edit! Created by nedtool 5.5 from nesting/linklayer/common/VLANTagR.msg.
//

#ifndef __NESTING_VLANTAGR_M_H
#define __NESTING_VLANTAGR_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// cplusplus {{
#include "inet/common/TagBase_m.h"
// }}


namespace nesting {

/**
 * Class generated from <tt>nesting/linklayer/common/VLANTagR.msg:25</tt> by nedtool.
 * <pre>
 * class VLANTagR extends inet::TagBase
 * {
 *     uint16_t uniqueID;
 *     uint32_t seqNum;
 * }
 * </pre>
 */
class VLANTagR : public ::inet::TagBase
{
  protected:
    uint16_t uniqueID;
    uint32_t seqNum;

  private:
    void copy(const VLANTagR& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VLANTagR&);

  public:
    VLANTagR();
    VLANTagR(const VLANTagR& other);
    virtual ~VLANTagR();
    VLANTagR& operator=(const VLANTagR& other);
    virtual VLANTagR *dup() const override {return new VLANTagR(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual uint16_t getUniqueID() const;
    virtual void setUniqueID(uint16_t uniqueID);
    virtual uint32_t getSeqNum() const;
    virtual void setSeqNum(uint32_t seqNum);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const VLANTagR& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, VLANTagR& obj) {obj.parsimUnpack(b);}

} // namespace nesting

#endif // ifndef __NESTING_VLANTAGR_M_H

