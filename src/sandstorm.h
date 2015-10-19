// Copyright (c) 2014-2015 The Darkcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SANDSTORM_H
#define SANDSTORM_H

//#include "primitives/transaction.h"
#include "main.h"
#include "stormnode.h"
#include "activestormnode.h"
#include "stormnodeman.h"

class CTxIn;
class CSandStormPool;
class CSandStormSigner;
class CStormNodeVote;
class CDarkSilkAddress;
class CSandstormQueue;
class CSandstormBroadcastTx;
class CActiveStormnode;

#define POOL_MAX_TRANSACTIONS                  3 // wait for X transactions to merge and publish
#define POOL_STATUS_UNKNOWN                    0 // waiting for update
#define POOL_STATUS_IDLE                       1 // waiting for update
#define POOL_STATUS_QUEUE                      2 // waiting in a queue
#define POOL_STATUS_ACCEPTING_ENTRIES          3 // accepting entries
#define POOL_STATUS_FINALIZE_TRANSACTION       4 // master node will broadcast what it accepted
#define POOL_STATUS_SIGNING                    5 // check inputs/outputs, sign final tx
#define POOL_STATUS_TRANSMISSION               6 // transmit transaction
#define POOL_STATUS_ERROR                      7 // error
#define POOL_STATUS_SUCCESS                    8 // success

// status update message constants
#define STORMNODE_ACCEPTED                    1
#define STORMNODE_REJECTED                    0
#define STORMNODE_RESET                       -1

#define SANDSTORM_QUEUE_TIMEOUT                 120
#define SANDSTORM_SIGNING_TIMEOUT               30

static const int MIN_POOL_PEER_PROTO_VERSION = 60020; // minimum peer version accepted by SandStormPool

extern CSandStormPool sandStormPool;
extern CSandStormSigner sandStormSigner;
extern std::vector<CSandstormQueue> vecSandstormQueue;
extern std::string strStormNodePrivKey;
extern map<uint256, CSandstormBroadcastTx> mapSandstormBroadcastTxes;
extern CActiveStormnode activeStormnode;

//specific messages for the Sandstorm protocol
void ProcessMessageSandstorm(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);

// get the sandstorm chain depth for a given input
int GetInputSandstormRounds(CTxIn in, int rounds=0);


// An input in the sandstorm pool
class CSandStormEntryVin
{
public:
    bool isSigSet;
    CTxIn vin;

    CSandStormEntryVin()
    {
        isSigSet = false;
        vin = CTxIn();
    }
};

// A clients transaction in the sandstorm pool
class CSandStormEntry
{
public:
    bool isSet;
    std::vector<CSandStormEntryVin> sev;
    int64_t amount;
    CTransaction collateral;
    std::vector<CTxOut> vout;
    CTransaction txSupporting;
    int64_t addedTime;

    CSandStormEntry()
    {
        isSet = false;
        collateral = CTransaction();
        amount = 0;
    }

    bool Add(const std::vector<CTxIn> vinIn, int64_t amountIn, const CTransaction collateralIn, const std::vector<CTxOut> voutIn)
    {
        if(isSet){return false;}

        BOOST_FOREACH(const CTxIn v, vinIn) {
            CSandStormEntryVin s = CSandStormEntryVin();
            s.vin = v;
            sev.push_back(s);
        }
        vout = voutIn;
        amount = amountIn;
        collateral = collateralIn;
        isSet = true;
        addedTime = GetTime();

        return true;
    }

    bool AddSig(const CTxIn& vin)
    {
        BOOST_FOREACH(CSandStormEntryVin& s, sev) {
            if(s.vin.prevout == vin.prevout && s.vin.nSequence == vin.nSequence){
                if(s.isSigSet){return false;}
                s.vin.scriptSig = vin.scriptSig;
                s.vin.prevPubKey = vin.prevPubKey;
                s.isSigSet = true;

                return true;
            }
        }

        return false;
    }

    bool IsExpired()
    {
        return (GetTime() - addedTime) > SANDSTORM_QUEUE_TIMEOUT;// 120 seconds
    }
};

//
// A currently inprogress sandstorm merge and denomination information
//
class CSandstormQueue
{
public:
    CTxIn vin;
    int64_t time;
    int nDenom;
    bool ready; //ready for submit
    std::vector<unsigned char> vchSig;

    CSandstormQueue()
    {
        nDenom = 0;
        vin = CTxIn();
        time = 0;
        vchSig.clear();
        ready = false;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion){
	unsigned int nSerSize = 0;
        READWRITE(nDenom);
        READWRITE(vin);
        READWRITE(time);
        READWRITE(ready);
        READWRITE(vchSig);
    }

    bool GetAddress(CService &addr)
    {
        CStormnode* psn = snodeman.Find(vin);
        if(psn != NULL)
        {
            addr = psn->addr;
            return true;
        }
        return false;
    }

    bool GetProtocolVersion(int &protocolVersion)
    {
        CStormnode* psn = snodeman.Find(vin);
        if(psn)
        {
            protocolVersion = psn->protocolVersion;
            return true;
        }
        return false;
    }

    bool Sign();
    bool Relay();

    bool IsExpired()
    {
        return (GetTime() - time) > SANDSTORM_QUEUE_TIMEOUT;// 120 seconds
    }

    bool CheckSignature();

};

// store sandstorm tx signature information
class CSandstormBroadcastTx
{
public:
    CTransaction tx;
    CTxIn vin;
    vector<unsigned char> vchSig;
    int64_t sigTime;
};

//
// Helper object for signing and checking signatures
//
class CSandStormSigner
{
public:
    bool IsVinAssociatedWithPubkey(CTxIn& vin, CPubKey& pubkey);
    bool SetKey(std::string strSecret, std::string& errorMessage, CKey& key, CPubKey& pubkey);
    bool SignMessage(std::string strMessage, std::string& errorMessage, std::vector<unsigned char>& vchSig, CKey key);
    bool VerifyMessage(CPubKey pubkey, std::vector<unsigned char>& vchSig, std::string strMessage, std::string& errorMessage);
};

class CSandstormSession
{

};

//
// Used to keep track of current status of sandstorm pool
//
class CSandStormPool
{
public:
    static const int MIN_PEER_PROTO_VERSION = 70020;

    // clients entries
    std::vector<CSandStormEntry> myEntries;
    // stormnode entries
    std::vector<CSandStormEntry> entries;
    // the finalized transaction ready for signing
    CTransaction finalTransaction;

    int64_t lastTimeChanged;
    int64_t lastAutoDenomination;

    unsigned int state;
    unsigned int entriesCount;
    unsigned int lastEntryAccepted;
    unsigned int countEntriesAccepted;

    // where collateral should be made out to
    CScript collateralPubKey;

    std::vector<CTxIn> lockedCoins;

    uint256 StormNodeBlockHash;

    std::string lastMessage;
    bool completedTransaction;
    bool unitTest;
    CService submittedToStormnode;

    int sessionID;
    int sessionDenom; //Users must submit an denom matching this
    int sessionUsers; //N Users have said they'll join
    bool sessionFoundStormnode; //If we've found a compatible stormnode
    int64_t sessionTotalValue; //used for autoDenom
    std::vector<CTransaction> vecSessionCollateral;

    int cachedLastSuccess;
    int cachedNumBlocks; //used for the overview screen
    int minBlockSpacing; //required blocks between mixes
    CTransaction txCollateral;

    int64_t lastNewBlock;

    //debugging data
    std::string strAutoDenomResult;

    //incremented whenever a DSQ comes through
    int64_t nSsqCount;

    CSandStormPool()
    {
        /* SandStorm uses collateral addresses to trust parties entering the pool
            to behave themselves. If they don't it takes their money. */

        cachedLastSuccess = 0;
        cachedNumBlocks = 0;
        unitTest = false;
        txCollateral = CTransaction();
        minBlockSpacing = 1;
        nSsqCount = 0;
        lastNewBlock = 0;

        SetNull();
    }

    void ProcessStormnodeConnections();

    void InitCollateralAddress(){
        std::string strAddress = "";
            strAddress = "C7FBJNGDmEsU5wx2m3xw85N8kRgCqA8S7L";
        SetCollateralAddress(strAddress);
    }

    void SetMinBlockSpacing(int minBlockSpacingIn){
        minBlockSpacing = minBlockSpacingIn;
    }

    bool SetCollateralAddress(std::string strAddress);
    void Reset();
    void SetNull(bool clearEverything=false);

    void UnlockCoins();

    bool IsNull() const
    {
        return (state == POOL_STATUS_ACCEPTING_ENTRIES && entries.empty() && myEntries.empty());
    }

    int GetState() const
    {
        return state;
    }

    int GetEntriesCount() const
    {
        if(fStormNode){
            return entries.size();
        } else {
            return entriesCount;
        }
    }

    int GetLastEntryAccepted() const
    {
        return lastEntryAccepted;
    }

    int GetCountEntriesAccepted() const
    {
        return countEntriesAccepted;
    }

    int GetMyTransactionCount() const
    {
        return myEntries.size();
    }

    void UpdateState(unsigned int newState)
    {
        if (fStormNode && (newState == POOL_STATUS_ERROR || newState == POOL_STATUS_SUCCESS)){
            LogPrintf("CSandStormPool::UpdateState() - Can't set state to ERROR or SUCCESS as a stormnode. \n");
            return;
        }

        LogPrintf("CSandStormPool::UpdateState() == %d | %d \n", state, newState);
        if(state != newState){
            lastTimeChanged = GetTimeMillis();
            if(fStormNode) {
                RelaySandStormStatus(sandStormPool.sessionID, sandStormPool.GetState(), sandStormPool.GetEntriesCount(), STORMNODE_RESET);
            }
        }
        state = newState;
    }

    int GetMaxPoolTransactions()
    {
        
        //use the production amount
        return POOL_MAX_TRANSACTIONS;
    }

    //Do we have enough users to take entries?
    bool IsSessionReady(){
        return sessionUsers >= GetMaxPoolTransactions();
    }

    // Are these outputs compatible with other client in the pool?
    bool IsCompatibleWithEntries(std::vector<CTxOut> vout);
    // Is this amount compatible with other client in the pool?
    bool IsCompatibleWithSession(int64_t nAmount, CTransaction txCollateral, std::string& strReason);

    // Passively run Sandstorm in the background according to the configuration in settings (only for Qt)
    bool DoAutomaticDenominating(bool fDryRun=false, bool ready=false);
    bool PrepareSandstormDenominate();


    // check for process in Sandstorm
    void Check();
    // charge fees to bad actors
    void ChargeFees();
    // rarely charge fees to pay miners
    void ChargeRandomFees();
    void CheckTimeout();
    // check to make sure a signature matches an input in the pool
    bool SignatureValid(const CScript& newSig, const CTxIn& newVin);
    // if the collateral is valid given by a client
    bool IsCollateralValid(const CTransaction& txCollateral);
    // add a clients entry to the pool
    bool AddEntry(const std::vector<CTxIn>& newInput, const int64_t& nAmount, const CTransaction& txCollateral, const std::vector<CTxOut>& newOutput, std::string& error);
    // add signature to a vin
    bool AddScriptSig(const CTxIn newVin);
    // are all inputs signed?
    bool SignaturesComplete();
    // as a client, send a transaction to a stormnode to start the denomination process
    void SendSandstormDenominate(std::vector<CTxIn>& vin, std::vector<CTxOut>& vout, int64_t amount);
    // get stormnode updates about the progress of sandstorm
    bool StatusUpdate(int newState, int newEntriesCount, int newAccepted, std::string& error, int newSessionID=0);

    // as a client, check and sign the final transaction
    bool SignFinalTransaction(CTransaction& finalTransactionNew, CNode* node);

    // get the last valid block hash for a given modulus
    bool GetLastValidBlockHash(uint256& hash, int mod=1, int nBlockHeight=0);
    // process a new block
    void NewBlock();
    void CompletedTransaction(bool error, std::string lastMessageNew);
    void ClearLastMessage();
    // used for liquidity providers
    bool SendRandomPaymentToSelf();
    // split up large inputs or make fee sized inputs
    bool MakeCollateralAmounts();
    bool CreateDenominated(int64_t nTotalValue);
    // get the denominations for a list of outputs (returns a bitshifted integer)
    int GetDenominations(const std::vector<CTxOut>& vout);
    void GetDenominationsToString(int nDenom, std::string& strDenom);
    // get the denominations for a specific amount of darksilk.
    int GetDenominationsByAmount(int64_t nAmount, int nDenomTarget=0);

    int GetDenominationsByAmounts(std::vector<int64_t>& vecAmount);
};


void ConnectToSandStormStormNodeWinner();

void ThreadCheckSandStormPool();

#endif
