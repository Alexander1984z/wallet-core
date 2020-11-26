// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "PrivateKey.h"
#include "Solana/Address.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Solana;

TEST(SolanaAddress, addressFromValidatorSeed) {
    auto user = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto validator = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    auto expected = Address("6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv");
    ASSERT_EQ(addressFromValidatorSeed(user, validator, programId), expected);
}

TEST(SolanaAddress, FromPublicKey) {
    const auto addressString = "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST";
    const auto publicKey = PublicKey(Base58::bitcoin.decode(addressString), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);
    ASSERT_EQ(addressString, address.string());
}

TEST(SolanaAddress, FromString) {
    string addressString = "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST";
    const auto address = Address(addressString);
    ASSERT_EQ(address.string(), addressString);
}

TEST(SolanaAddress, isValid) {
    ASSERT_TRUE(Address::isValid("2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST"));
    ASSERT_FALSE(Address::isValid(
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdSl")); // Contains invalid base-58 character
    ASSERT_FALSE(
        Address::isValid("2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpd")); // Is invalid length
}

TEST(SolanaAddress, isValidOnCurve) {
    EXPECT_TRUE(Address::isValidOnCurve(Base58::bitcoin.decode("68io7dTfyeWua1wD1YcCMka4y5iiChceaFRCBjqCM5PK")));
    EXPECT_TRUE(Address::isValidOnCurve(Base58::bitcoin.decode("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP")));
}

TEST(SolanaTokenProgram, defaultTokenAddress) {
    const std::string serumToken = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt";
    EXPECT_EQ(TokenProgram::defaultTokenAddress("HBYC51YrGFAZ8rM7Sj8e9uqKggpSrDYrinQDZzvMtqQp", serumToken),
        "68io7dTfyeWua1wD1YcCMka4y5iiChceaFRCBjqCM5PK");
    EXPECT_EQ(TokenProgram::defaultTokenAddress("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V", serumToken),
        "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
}

TEST(SolanaTokenProgram, findProgramAddress) {
    std::vector<Data> seeds = {
        Base58::bitcoin.decode("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V"),
        Base58::bitcoin.decode("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA"),
        Base58::bitcoin.decode("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"),
    };
    std::string address = TokenProgram::findProgramAddress(seeds, Address("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL"));
    EXPECT_EQ(address, "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
}

TEST(SolanaTokenProgram, createProgramAddress) {
    std::vector<Data> seeds = {
        Base58::bitcoin.decode("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V"),
        Base58::bitcoin.decode("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA"),
        Base58::bitcoin.decode("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"),
        Data{255}
    };
    Address address = TokenProgram::createProgramAddress(seeds, Address("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL"));
    EXPECT_EQ(address.string(), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
}
