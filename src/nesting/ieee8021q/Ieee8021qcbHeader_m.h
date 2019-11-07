//
// Generated file, do not edit! Created by nedtool 5.5 from nesting/ieee8021q/Ieee8021qcbHeader.msg.
//

#ifndef __NESTING_IEEE8021QCBHEADER_M_H
#define __NESTING_IEEE8021QCBHEADER_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif


namespace nesting {

class Ieee8021qcbHeader;
} // namespace nesting

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk


namespace nesting {

/**
 * Class generated from <tt>nesting/ieee8021q/Ieee8021qcbHeader.msg:24</tt> by nedtool.
 * <pre>
 * class Ieee8021qcbHeader extends inet::FieldsChunk
 * {
 *     chunkLength = inet::B(6);
 *     uint16_t uniqueID;
 *     uint32_t seqNum;
 * }
 * </pre>
 */
class Ieee8021qcbHeader : public ::inet::FieldsChunk
{
  protected:
    uint16_t uniqueID = 0;
    uint32_t seqNum = 0;

  private:
    void copy(const Ieee8021qcbHeader& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee8021qcbHeader&);

  public:
    Ieee8021qcbHeader();
    Ieee8021qcbHeader(const Ieee8021qcbHeader& other);
    virtual ~Ieee8021qcbHeader();
    Ieee8021qcbHeader& operator=(const Ieee8021qcbHeader& other);
    virtual Ieee8021qcbHeader *dup() const override {return new Ieee8021qcbHeader(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual uint16_t getUniqueID() const;
    virtual void setUniqueID(uint16_t uniqueID);
    virtual uint32_t getSeqNum() const;
    virtual void setSeqNum(uint32_t seqNum);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Ieee8021qcbHeader& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Ieee8021qcbHeader& obj) {obj.parsimUnpack(b);}

} // namespace nesting

#endif // ifndef __NESTING_IEEE8021QCBHEADER_M_H

