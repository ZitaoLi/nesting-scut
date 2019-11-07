//
// Generated file, do not edit! Created by nedtool 5.5 from nesting/linklayer/common/VLANTag.msg.
//

#ifndef __NESTING_VLANTAG_M_H
#define __NESTING_VLANTAG_M_H

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
 * Class generated from <tt>nesting/linklayer/common/VLANTag.msg:25</tt> by nedtool.
 * <pre>
 * class VLANTagBase extends inet::TagBase
 * {
 *     uint8_t pcp;
 *     bool de;
 *     short VID = 0;
 * }
 * </pre>
 */
class VLANTagBase : public ::inet::TagBase
{
  protected:
    uint8_t pcp;
    bool de;
    short VID;

  private:
    void copy(const VLANTagBase& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VLANTagBase&);

  public:
    VLANTagBase();
    VLANTagBase(const VLANTagBase& other);
    virtual ~VLANTagBase();
    VLANTagBase& operator=(const VLANTagBase& other);
    virtual VLANTagBase *dup() const override {return new VLANTagBase(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual uint8_t getPcp() const;
    virtual void setPcp(uint8_t pcp);
    virtual bool getDe() const;
    virtual void setDe(bool de);
    virtual short getVID() const;
    virtual void setVID(short VID);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const VLANTagBase& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, VLANTagBase& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>nesting/linklayer/common/VLANTag.msg:35</tt> by nedtool.
 * <pre>
 * //
 * // Determines VLAN 802.1Q parameters for an outgoing packet.
 * //
 * class VLANTagReq extends VLANTagBase
 * {
 * }
 * </pre>
 */
class VLANTagReq : public ::nesting::VLANTagBase
{
  protected:

  private:
    void copy(const VLANTagReq& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VLANTagReq&);

  public:
    VLANTagReq();
    VLANTagReq(const VLANTagReq& other);
    virtual ~VLANTagReq();
    VLANTagReq& operator=(const VLANTagReq& other);
    virtual VLANTagReq *dup() const override {return new VLANTagReq(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const VLANTagReq& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, VLANTagReq& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>nesting/linklayer/common/VLANTag.msg:43</tt> by nedtool.
 * <pre>
 * //
 * // Specifies VLAN 802.1Q parameters for an incoming packet.
 * //
 * class VLANTagInd extends VLANTagBase
 * {
 * }
 * </pre>
 */
class VLANTagInd : public ::nesting::VLANTagBase
{
  protected:

  private:
    void copy(const VLANTagInd& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VLANTagInd&);

  public:
    VLANTagInd();
    VLANTagInd(const VLANTagInd& other);
    virtual ~VLANTagInd();
    VLANTagInd& operator=(const VLANTagInd& other);
    virtual VLANTagInd *dup() const override {return new VLANTagInd(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const VLANTagInd& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, VLANTagInd& obj) {obj.parsimUnpack(b);}

} // namespace nesting

#endif // ifndef __NESTING_VLANTAG_M_H

