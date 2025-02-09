#pragma once

#include "ConfigParse/ConfigParse.hpp"
#include "Server/ComputationToComputationContainer/Server.hpp"
#include "FixedPoint/FixedPoint.hpp"
#include "PrimeField/PrimeField.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <iterator>
#include <thread>
#include <vector>
#include "AddressId.hpp"
namespace qmpc::Share
{
template <typename T>
class Share;

template <typename>
struct is_share_vector : std::false_type
{
};
template <typename T, typename ALLOCATOR>
struct is_share_vector<std::vector<Share<T>, ALLOCATOR>> : std::true_type
{
};

template <typename>
struct is_share : std::false_type
{
};
template <typename T>
struct is_share<Share<T>> : std::true_type
{
};
template <typename T>
void send(const T &shares, const int &pt_id)
{
    if constexpr (std::disjunction_v<std::is_array<T>, is_share_vector<T>>)
    {
        // pt_idで指定されたパーティにシェアの値を送信する
        Config *conf = Config::getInstance();
        auto client = ComputationToComputation::Server::getServer()->getClient(pt_id);

        size_t length = std::size(shares);
        std::vector<std::string> str_values(length);
        std::vector<qmpc::Share::AddressId> share_ids(length);
        for (unsigned int i = 0; i < length; i++)
        {
            str_values[i] = shares[i].getVal().getStrVal();
            share_ids[i] = shares[i].getId();
        }
        client->exchangeShares(str_values.data(), share_ids.data(), length, conf->party_id);
    }
    else if constexpr (is_share<T>::value)
    {
        Config *conf = Config::getInstance();
        // pt_idで指定されたパーティにシェアの値を送信する
        auto client = ComputationToComputation::Server::getServer()->getClient(pt_id);
        client->exchangeShare(shares.getVal().getStrVal(), shares.getId(), conf->party_id);
    }
}
template <class SV>
void send(const SV &share_value, const AddressId &share_id, int pt_id)
{
    Config *conf = Config::getInstance();
    auto client = ComputationToComputation::Server::getServer()->getClient(pt_id);
    client->exchangeShare(share_value.getStrVal(), share_id, conf->party_id);
}
template <typename T>
void open(const T &share)
{
    Config *conf = Config::getInstance();
    std::vector<std::thread> threads;
    // 全パーティにシェアの値を送信する
    for (int pt_id = 1; pt_id <= conf->n_parties; pt_id++)
    {
        if (pt_id != conf->party_id)
        {
            threads.push_back(std::thread([=, &share]() { send(share, pt_id); }));
        }
    }

    for (auto &th : threads)
    {
        th.join();
    }
}

template <typename T, std::enable_if_t<is_share<T>::value, std::nullptr_t> = nullptr>
auto recons(const T &share)
{
    Config *conf = Config::getInstance();
    // シェア保有者が使うrecons関数
    auto server = ComputationToComputation::Server::getServer();
    //単一
    using Result = typename T::value_type;
    Result ret;
    for (int pt_id = 1; pt_id <= conf->n_parties; pt_id++)
    {
        if (pt_id == conf->party_id)
        {
            ret += share.getVal();
        }
        else
        {
            Result received_value{server->getShare(pt_id, share.getId())};
            ret += received_value;
        }
    }
    return ret;
}
template <typename T, std::enable_if_t<is_share_vector<T>::value, std::nullptr_t> = nullptr>
auto recons(const T &share)
{
    Config *conf = Config::getInstance();
    // シェア保有者が使うrecons関数
    auto server = ComputationToComputation::Server::getServer();
    //一括部分
    using Result = typename T::value_type::value_type;
    size_t length = std::size(share);
    std::vector<Result> ret(length);
    for (int pt_id = 1; pt_id <= conf->n_parties; pt_id++)
    {
        std::vector<qmpc::Share::AddressId> ids_list(length); // 複数シェアのidリスト
        for (unsigned int i = 0; i < length; i++)
        {
            ids_list[i] = share[i].getId();
        }

        if (pt_id == conf->party_id)
        {
            for (unsigned int i = 0; i < length; i++)
            {
                ret[i] += share[i].getVal();
            }
        }
        else
        {
            std::vector<std::string> values = server->getShares(pt_id, ids_list, length);
            for (unsigned int i = 0; i < length; i++)
            {
                ret[i] += Result(values[i]);
            }
        }
    }

    return ret;
}
template <typename SV>
SV receive(int sp_id, AddressId address_id)
{
    ComputationToComputation::Server *server = ComputationToComputation::Server::getServer();
    SV received_value(server->getShare(sp_id, address_id));
    return received_value;
}
}  // namespace qmpc::Share