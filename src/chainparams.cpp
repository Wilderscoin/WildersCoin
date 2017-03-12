// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0xce;
        pchMessageStart[2] = 0x73;
        pchMessageStart[3] = 0x2d;
        vAlertPubKey = ParseHex("09478e96c9056be6b61e659c04b94acefeac5d5257fe028e80695c62f7c2f81f85d131a446df3be611393f454852a2d08c6314aad5ca3cbe5616262db3d4a6efac");
        nDefaultPort = 12767;
        nRPCPort = 12768;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);

        const char* pszTimestamp = "http://www.reuters.com/article/us-turkey-europe-netherlands-idUSKBN16I07O"; // Saturday, 11  2017
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1489271886, vin, vout, 0); // (GMT): Fri, 07 Oct 2016 23:23:23 GMT
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1489271886; // (GMT): Fri, 07 Oct 2016 23:23:23 GMT
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 3567490;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xa93fa689d523788e58cd9901c08d6a545f15ed41cc9cc51e971eca092bcd27cf"));
        assert(genesis.hashMerkleRoot == uint256("0xedafebb487c0248a6c112fbb82d9e5b43cd09fb80b093d6f770b86ba9cfad1df"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,31);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,88);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xA4)(0x7E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0x0F)(0x3D).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nTargetSpacing = 60; // 60 Sec during PoW
        if(nBestHeight > nLastPoWBlock) // Scaled down for PoS only phase
        {
          nTargetSpacing = 120;
        }
        if(nBestHeight > nStartPoSBlock) // Scaled up for PoW/PoS twin phase
        {
          if(nBestHeight <= nLastPoWBlock)
          {
            nTargetSpacing = 240;
          }
        }
        nTargetTimespan = 10 * nTargetSpacing;
        nLastPoWBlock = 500000;
        nStartPoSBlock = 1;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xa0;
        pchMessageStart[1] = 0xb4;
        pchMessageStart[2] = 0x26;
        pchMessageStart[3] = 0xd2;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 16);

        vAlertPubKey = ParseHex("094acefeac5d5257fe028e80695c62f7c2f1e69478e96c9056be6b659c04b81f85d131a446df3be611393f454852a2d08c1687ecf5ca3cbe5616262db3d4a6efac");
        nDefaultPort = 22768;
        nRPCPort = 22767;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 97642;

        hashGenesisBlock = genesis.GetHash();
        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,58);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,27);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x06)(0x88)(0xD3)(0x2a).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x06)(0x88)(0xBC)(0xE8).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nTargetTimespan = 10 * nTargetSpacing;
        nLastPoWBlock = 500000;
        nStartPoSBlock = 1;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0x00;
        pchMessageStart[1] = 0xcc;
        pchMessageStart[2] = 0x87;
        pchMessageStart[3] = 0xca;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1489271886;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 8;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 23674;
        strDataDir = "regtest";

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
