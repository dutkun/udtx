/*****************************************************************************
 Copyright (c) 2001 - 2015, The Board of Trustees of the University of Illinois.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * Redistributions of source code must retain the above
 copyright notice, this list of conditions and the
 following disclaimer.

 * Redistributions in binary form must reproduce the
 above copyright notice, this list of conditions
 and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of the University of Illinois
 nor the names of its contributors may be used to
 endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/
/*****************************************************************************
 written by
 fan lielong, last updated 04/13/2015
 *****************************************************************************/

#ifndef __UDT_CUBICCC_H__
#define __UDT_CUBICCC_H__

#include "ccc.h"

class CUBICCC: public CCC
{
public:
    CUBICCC();
public:
    virtual void init();
    virtual void registerLinuxprotocol(struct tcp_congestion_ops *linuxtco);
    virtual void updatesock();  //update tcp_sock struct
    virtual void onACK(int32_t);
    virtual void onLoss(const int32_t*, int);
    virtual void onTimeout();
    virtual void statemachine(int32_t); //from kernel tcp_fastretrans_alert (in future)
    //virtual void tcp_reno_cong_avoid(const struct sock* sk, int ack, int inflight);   //reno cong avoid action *additive increase*
    //virtual void tcp_reno_min_cwnd(const struct sock* sk);    //reno min cwnd action
    virtual void updateudt();
private:
    int m_issthresh;        // 慢启动阈值；slowstart threshold
    bool m_bSlowStart;      // 是否再慢启动阶段；see if in slowstart phase
    int m_iDupACKCount;     // 重复ACK个数；count DupAck
    int m_iLastACK;         // 最新ACK序列号；Last Ack Seq no
    unsigned char ca_state; // TCP的状态；TCP state
    struct sock sk;
    struct tcp_congestion_ops *ca_ops;

private:
    int m_iRCInterval;           // UDT Rate control interval
    uint64_t m_LastRCTime;       // last rate increase time
    //bool m_bSlowStart;           // if in slow start phase
    int32_t m_iLastAck;          // last ACKed seq no
    bool m_bLoss;                // if loss happened since last rate increase
    int32_t m_iLastDecSeq;       // max pkt seq no sent out when last decrease happened
    double m_dLastDecPeriod;     // value of pktsndperiod when last decrease happened
    int m_iNAKCount;             // NAK counter
    int m_iDecRandom;            // random threshold on decrease by number of loss events
    int m_iAvgNAKNum;            // average number of NAKs per congestion
    int m_iDecCount;             // number of decreases in a congestion epoch
};

#endif /* __UDT_CUBICCC_H__ */
