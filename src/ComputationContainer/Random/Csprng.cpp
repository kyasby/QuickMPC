#include "Random/Csprng.hpp"
#include "LogHeader/Logger.hpp"
#include "Logging/Logger.hpp"
// randombytes_buf_deterministic の内部はChaCha20
// ((2**32[ctrが4byte長])*64[byte/block])/10**9[byte->GB] = 274GB[俗にいう256GB]
// 内部ctrの範囲が1~2**32なので、1つのkey(seed)で274GB分のRNが生成できる

namespace Utility
{
bool CSPRNG::entropyCheck()
{
    while (sodium_init() < 0)
    {
        static unsigned int ctr = 0;
        ctr++;
        if (ctr > 3000)  // エントロピーが出来上がるのに最大3分程度待つ
        {
            spdlog::info("There is a serious lack of entropy.");
            // 例外を送出
            // throw "There is a serious lack of entropy.";
            return false;
        }
        else if (ctr % 1000 == 0)  // 10秒溜まってなかったら1分待つ
        {
            // あまりにもエントロピーがたまらないので1分待機
            spdlog::info("Standing by for one minute due to lack of entropy...");
            std::this_thread::sleep_for(std::chrono::minutes(1));  // 1分待機
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));  // 3 マイクロ秒待機
    }
    return true;
};

void CSPRNG::GetRand(unsigned char *buf, unsigned int byteSize)
{
    if (!this->CSPRNG::entropyCheck())
    {
        throw "There is a serious lack of entropy.";
        qmpc::Log::throw_with_trace(
            std::runtime_error("RandomError: There is a serious lack of entropy.")
        );
    }
    if (byteSize
        <= 32)  // seedで32byteの乱数を使うので、seed以下のサイズの場合はseedを乱数として返す。
    {
        randombytes_buf(buf, byteSize);
    }
    else
    {
        unsigned char *seed = new unsigned char[randombytes_SEEDBYTES];
        syscall(SYS_getrandom, seed, randombytes_SEEDBYTES, 1);
        randombytes_buf_deterministic(buf, byteSize, seed);
    }
};

long long int CSPRNG::GetRandLL()
{
    constexpr unsigned int LL_SIZE = sizeof(long long int);  // 8byte = 64bit

    unsigned char *rnd = new unsigned char[LL_SIZE];
    // 64bit乱数[unsigned char*]生成
    this->CSPRNG::GetRand(rnd, LL_SIZE);

    // unsiged char* -> str(bin)
    std::stringstream str;
    for (unsigned int i = 0; i < LL_SIZE; i++)
    {
        str << std::bitset<LL_SIZE>(rnd[i]);
    }
    std::string rndBinStr = str.str();

    long long int rndVal = std::stoull(rndBinStr, nullptr, 2);
    return rndVal;
};

std::vector<long long int> CSPRNG::GetRandLLVec(unsigned int size)
{
    std::vector<long long int> randLLVec = {};

    constexpr unsigned int LL_SIZE = sizeof(long long int);  // 8byte
    const unsigned int byteSize = size * LL_SIZE;            // size * 8[byte/llsize]

    unsigned char *rnd = new unsigned char[byteSize];
    // 64bit乱数[unsigned char*]生成
    this->CSPRNG::GetRand(rnd, byteSize);

    // unsiged char* -> str(bin)
    for (unsigned int i = 0; i < byteSize; i += LL_SIZE)
    {
        std::stringstream str;
        for (unsigned int j = 0; j < LL_SIZE; j++)
        {
            str << std::bitset<LL_SIZE>(rnd[i + j]);
        }

        long long int rndVal = std::stoull(str.str(), nullptr, 2);
        randLLVec.push_back(rndVal);
    }

    return randLLVec;
};
}  // namespace Utility
