#pragma once

#include <cmath>
#include "ConfigParse/ConfigParse.hpp"
#include "Random/Random.hpp"
#include "Networking.hpp"
#include <boost/operators.hpp>
#include <atomic>
#include <tuple>
#include <string>
#include "AddressId.hpp"
#include "TripleHandler/TripleHandler.hpp"
#include "Logging/Logger.hpp"
namespace qmpc::Share
{
template <typename SV>
class Share : boost::operators<Share<SV>>,
              boost::addable<Share<SV>, SV>,
              boost::subtractable<Share<SV>, SV>,
              boost::multipliable<Share<SV>, SV>,
              boost::dividable<Share<SV>, SV>,
              boost::equality_comparable<Share<SV>, SV>,
              boost::less_than_comparable<Share<SV>, SV>
{
private:
    SV value;
    AddressId address_id;

public:
    using value_type = SV;
    // TODO:Tripleが正式に実装されたら削除すること
    using Triple = std::tuple<SV, SV, SV>;
    Share() noexcept : value(), address_id() {}
    Share(const Share &s) noexcept : value(s.value), address_id() {}
    Share(Share &&s) noexcept : value(std::move(s.value)), address_id(std::move(s.address_id)) {}
    Share(const SV &value) noexcept : value(value), address_id() {}
    Share &operator=(const Share &obj)
    {
        value = obj.value;
        return *this;
    }
    Share &operator=(Share &&obj)
    {
        value = std::move(obj.value);
        return *this;
    }
    Share &operator=(const SV &obj)
    {
        value = obj;
        return *this;
    }
    SV getVal() const noexcept { return value; }
    double getDoubleVal() const { return value.getDoubleVal(); }
    auto getId() const noexcept { return address_id; }
    Share operator-() const
    {
        Share ret{*this};
        ret.value = -ret.value;
        return ret;
    }
    friend SV operator%(const Share &left, const SV &right)
    {
        // 事前準備 (乱数範囲の設定)
        // オーバフローしない範囲を計算
        long long int rand_max_value =
            (long long int)(right.getMaxInt() / std::stod(right.getStrVal()));
        long long int rand_min_value = -rand_max_value;
        if (rand_min_value > rand_max_value)
        {
            std::swap(rand_min_value, rand_max_value);
        }

        // step1: 乱数kを取得し、r_shareを計算
        Share k =
            Share(RandGenerator::getInstance()->getRand<FixedPoint>(rand_min_value, rand_max_value)
            );
        Share r_share = k * right - left;

        // step2: r_shareをopenにする。
        open(r_share);
        SV r_recons = recons(r_share);

        // step3: a mod p = p*{-r/p} を計算する。
        double p = std::stod(right.getStrVal());
        double r = std::stod(r_recons.getStrVal());
        double tmp = -(r / p);
        double round_value = std::round(p * (tmp - std::floor(tmp)));
        SV res(std::to_string(round_value));

        return res;
    }

    Share &operator+=(const Share &obj)
    {
        value += obj.value;
        return *this;
    }
    Share &operator-=(const Share &obj)
    {
        value -= obj.value;
        return *this;
    }
    Share &operator*=(const Share &obj)
    {
        // Beaver Triplet a, b, c のシェア [a], [b], [c] を得る
        auto t = qmpc::TripleHandler::TripleHandler::getInstance()->getTriple<SV>();
        Share a(std::get<0>(t[0])), b(std::get<1>(t[0])), c(std::get<2>(t[0]));

        // [d] = [x] - [a], [e] = [y] - [b] を計算する
        Share d;
        d = (*this) - a;
        Share e;
        e = obj - b;
        std::vector<Share> de = {(*this) - a, obj - b};
        // [d], [e] を全パーティに送信
        open(de);

        // d, e を受け取って復元する
        std::vector<SV> de_rec = recons(de);

        // [xy] = [c] + d[b] + e[a] + de を計算する
        Share v;
        v = c + de_rec[0] * b + de_rec[1] * a + de_rec[0] * de_rec[1];
        *this = v;

        return *this;
    }
    Share &operator/=(const Share &obj)
    {
        *this *= getInv(obj);
        return *this;
    }

    Share &operator+=(const SV &obj)
    {
        Config *conf = Config::getInstance();
        if (conf->party_id == conf->sp_id)
        {
            value += obj;
        }
        return *this;
    }
    Share &operator-=(const SV &obj)
    {
        Config *conf = Config::getInstance();
        if (conf->party_id == conf->sp_id)
        {
            value -= obj;
        }
        return *this;
    }
    Share &operator*=(const SV &obj)
    {
        value *= obj;
        return *this;
    }
    friend std::vector<Share> operator+(const std::vector<Share> &left, const SV &right)
    {
        std::vector<Share> ret;
        ret.reserve(left.size());

        for (const auto &v : left)
        {
            ret.emplace_back(v + right);
        }

        return ret;
    }
    friend std::vector<Share> operator+(const SV &left, const std::vector<Share> &right)
    {
        return right + left;
    }
    friend std::vector<Share> operator-(const std::vector<Share> &left, const SV &right)
    {
        return left + (-right);
    }
    friend std::vector<Share> operator-(const SV &left, const std::vector<Share> &right)
    {
        return left + getAdditiveInvVec(right);
    }
    friend std::vector<Share> operator*(const std::vector<Share> &left, const SV &right)
    {
        std::vector<Share> ret;
        ret.reserve(left.size());

        for (const auto &v : left)
        {
            ret.emplace_back(v * right);
        }

        return ret;
    }
    friend std::vector<Share> operator*(const SV &left, const std::vector<Share> &right)
    {
        return right * left;
    }
    friend std::vector<Share> operator/(const std::vector<Share> &left, const SV &right)
    {
        return left * (SV(1.0) / right);
    }
    friend std::vector<Share> operator/(const SV &left, const std::vector<Share> &right)
    {
        return left * getMultiplicativeInvVec(right);
    }
    Share &operator/=(const SV &obj)
    {
        value /= obj;
        return *this;
    }
    friend Share operator-(const SV &left, const Share &right)
    {
        Share ret{-right};
        ret += left;
        return ret;
    }
    friend Share operator/(const SV &left, const Share &right)
    {
        Share ret = getInv(right);
        ret *= left;
        return ret;
    }
    friend bool operator<(const Share<FixedPoint> &left, const Share<FixedPoint> &right);
    friend bool operator==(const Share<FixedPoint> &left, const Share<FixedPoint> &right);
    friend bool operator<(const Share<FixedPoint> &left, const FixedPoint &right);
    friend bool operator==(const Share<FixedPoint> &left, const FixedPoint &right);

    friend std::vector<Share> operator+(
        const std::vector<Share> &obj1, const std::vector<Share> &obj2
    )
    {
        size_t n = obj1.size();
        if (n != obj2.size())
        {
            qmpc::Log::throw_with_trace(
                std::runtime_error("Not Equal Length operator vector<Share> !!")
            );
        }

        std::vector<Share> ret;
        ret.reserve(obj1.size());
        for (int i = 0; i < static_cast<int>(n); ++i)
        {
            ret.emplace_back(obj1[i] + obj2[i]);
        }
        return ret;
    }

    friend std::vector<Share> operator-(
        const std::vector<Share> &obj1, const std::vector<Share> &obj2
    )
    {
        size_t n = obj1.size();
        if (n != obj2.size())
        {
            qmpc::Log::throw_with_trace(
                std::runtime_error("Not Equal Length operator vector<Share> !!")
            );
        }

        std::vector<Share> ret;
        ret.reserve(obj1.size());
        for (int i = 0; i < static_cast<int>(n); ++i)
        {
            ret.emplace_back(obj1[i] - obj2[i]);
        }
        return ret;
    }

    friend std::vector<Share> operator*(
        const std::vector<Share> &obj1, const std::vector<Share> &obj2
    )
    {
        size_t n = obj1.size();
        if (n != obj2.size())
        {
            qmpc::Log::throw_with_trace(
                std::runtime_error("Not Equal Length operator vector<Share> !!")
            );
        }
        // Beaver Triplet a, b, c のシェア [a], [b], [c] を得る
        auto t = qmpc::TripleHandler::TripleHandler::getInstance()->getTriple<SV>(n);

        std::vector<Share> a, b, c;
        a.reserve(n);
        b.reserve(n);
        c.reserve(n);
        for (size_t i = 0; i < n; ++i)
        {
            a.emplace_back(Share(std::get<0>(t[i])));
            b.emplace_back(Share(std::get<1>(t[i])));
            c.emplace_back(Share(std::get<2>(t[i])));
        }
        // [d] = [x] - [a], [e] = [y] - [b] を計算する
        std::vector<Share> de(n * 2);
        for (size_t i = 0; i < n; ++i)
        {
            de[i] = obj1[i] - a[i];
            de[i + n] = obj2[i] - b[i];
        }

        // [d], [e] を全パーティに送信
        open(de);

        // d, e を受け取って復元する
        std::vector<SV> de_rec = recons(de);

        // [xy] = [c] + d[b] + e[a] + de を計算する
        std::vector<Share> v(n);
        for (size_t i = 0; i < n; ++i)
        {
            v[i] = c[i] + de_rec[i] * b[i] + de_rec[n + i] * a[i] + de_rec[i] * de_rec[n + i];
        }

        return v;
    }
};

template <class T>
Share<T> sharize(const T &secret)
{
    Config *conf = Config::getInstance();
    Share<T> s;
    if (conf->party_id == conf->sp_id)
    {
        T r_sum;
        std::string ip_addr;
        for (int pt_id = 1; pt_id <= conf->n_parties; pt_id++)
        {
            if (pt_id == conf->sp_id)
            {
                continue;
            }
            auto r = RandGenerator::getInstance()->getRand<T>(1, 1000);

            r_sum += r;
            send(r, s.getId(), pt_id);
        }
        s = secret - r_sum;
    }
    else
    {
        s = receive<T>(conf->sp_id, s.getId());
    }
    return s;
}

template <typename T>
Share<T> getInv(const Share<T> &s)
{
    Config *conf = Config::getInstance();
    Share<T> r =
        Share(RandGenerator::getInstance()->getRand<T>(1, 1000));  // TODO: 範囲を適切に制限する
    auto u = s * r;
    Share<T> s_inv;

    if (conf->party_id == conf->sp_id)
    {
        T u_rec = recons(u);
        s_inv = sharize(u_rec.getInv());
    }
    else
    {
        send(u, conf->sp_id);
        s_inv = sharize(T());
    }
    s_inv = s_inv * r;

    return s_inv;
}

// vector の各要素の加法逆元を求める。
template <typename T>
std::vector<Share<T>> getAdditiveInvVec(const std::vector<Share<T>> &s)
{
    std::vector<Share<T>> ret;
    ret.reserve(s.size());
    for (const auto &v : s)
    {
        ret.emplace_back(-v);
    }
    return ret;
}

// vector の各要素の乗法逆元を求める。
template <typename T>
std::vector<Share<T>> getMultiplicativeInvVec(const std::vector<Share<T>> &s)
{
    std::vector<Share<T>> ret;
    ret.reserve(s.size());
    for (const auto &v : s)
    {
        ret.emplace_back(T(1.0) / v);
    }
    return ret;
}

template <typename T>
Share<T> getConstantShare(const T &t)
{
    Share<T> s;
    s += t;
    return s;
}
/*
入力されたShareの平方根を求める関数
入力値　真値が正の数のみ（負の数はNG）、内部で判定できないので実装時に注意
*/
template <typename T>
Share<T> sqrt(const Share<T> &share)
{
    // epsの代わり
    Share<T> x = (share + FixedPoint(1)) / FixedPoint(2);
    //回数は適当
    for (size_t i = 0; i < 50; ++i)
    {
        x = (x + share / x) / FixedPoint(2);
    }
    return x;
}

template <typename T>
std::vector<Share<T>> getRandShares(long long min_val, long long max_val, int n)
{
    std::vector<Share<T>> ret;
    ret.reserve(n);

    std::vector<FixedPoint> r = RandGenerator::getInstance()->getRandVec<T>(min_val, max_val, n);
    for (const auto &v : r)
    {
        ret.emplace_back(Share(v));
    }
    return ret;
}

template <typename T>
Share<T> getRandBitShare()
{
    while (true)
    {
        Share<T> r = Share(RandGenerator::getInstance()->getRand<T>(-1000, 1000));
        Share<T> square_r = r * r;
        open(square_r);
        T square_r_rec = recons(square_r);
        if (square_r_rec.getDoubleVal() != 0.0)
        {
            T r_dash = T(std::sqrt(square_r_rec.getDoubleVal()));
            T inv_r_dash = T(1.0) / r_dash;
            Share<T> r0 = Share(T(0.5) * (inv_r_dash * r + T(1.0)));
            return r0;
        }
    }
}

template <typename T>
std::vector<Share<T>> getRandBitShare(int n)
{
    std::vector<Share<T>> r = getRandShares<T>(-1000, 1000, n);
    std::vector<Share<T>> square_r = r * r;
    open(square_r);
    std::vector<T> square_r_rec = recons(square_r);

    std::vector<Share<T>> ret;
    ret.reserve(n);
    for (int i = 0; i < n; i++)
    {
        if (square_r_rec[i].getDoubleVal() != 0.0)
        {
            T r_dash = T(std::sqrt(square_r_rec[i].getDoubleVal()));
            T inv_r_dash = T(1.0) / r_dash;
            Share<T> r0 = Share(T(0.5) * (inv_r_dash * r[i] + T(1.0)));
            ret.emplace_back(r0);
        }
        else
        {
            // もし square_r_rec[i] の値が 0
            // である場合は、再度乱数ビットシェアを生成する
            ret.emplace_back(getRandBitShare<T>());
        }
    }
    return ret;
}

template <typename T>
Share<T> getLSBShare(const Share<T> &y)
{
    Share<T> r0 = getRandBitShare<T>();
    Share<T> r_dash = Share(RandGenerator::getInstance()->getRand<T>(1, 1000));
    Share<T> t = y + r0 + T(2) * r_dash;
    open(t);
    T c = recons(t);
    c = T(std::round(c.getDoubleVal())) % T(2);
    Share<T> b = c + r0 - T(2) * c * r0;
    return b;
}

template <typename T>
std::vector<Share<T>> getLSBShare(const std::vector<Share<T>> &y)
{
    std::vector<Share<T>> r0 = getRandBitShare<T>(static_cast<int>(y.size()));
    std::vector<Share<T>> r_dash = getRandShares<T>(0, 1000, static_cast<int>(y.size()));
    std::vector<Share<T>> t = y + r0 + T(2) * r_dash;
    open(t);
    std::vector<T> c = recons(t);
    std::vector<Share<T>> b;
    b.reserve(y.size());
    for (int j = 0; j < static_cast<int>(c.size()); ++j)
    {
        c[j] = T(std::round(c[j].getDoubleVal())) % T(2);
        b.emplace_back(c[j] + r0[j] - T(2) * c[j] * r0[j]);
    }

    return b;
}

template <typename T>
Share<T> getFloor(const Share<T> &s)
{
    // m, k は誤差が小さくなる値を設定している。
    // これ以上値を大きくすると、誤差が大きくなる可能性があるので注意。
    int m = 16;
    int k = 32;

    Share<T> x = s * T(std::pow(2, m));
    Share<T> y = T(std::pow(2, k)) + x;
    for (int i = 0; i < m; ++i)
    {
        Share<T> b = getLSBShare(y);
        y = (y - b) * T(0.5);
    }
    return y - T(std::pow(2, k - m));
}

template <typename T>
std::vector<Share<T>> getFloor(const std::vector<Share<T>> &s)
{
    // m, k は誤差が小さくなる値を設定している。
    // これ以上値を大きくすると、誤差が大きくなる可能性があるので注意。
    int m = 16;
    int k = 32;
    std::vector<Share<T>> x = s * T(std::pow(2, m));
    std::vector<Share<T>> y = T(std::pow(2, k)) + x;
    std::vector<Share<T>> ret;
    ret.reserve(s.size());
    for (int i = 0; i < m; ++i)
    {
        std::vector<Share<T>> b = getLSBShare(y);
        y = (y - b) * T(0.5);
    }
    for (int j = 0; j < static_cast<int>(y.size()); ++j)
    {
        ret.emplace_back(y[j] - T(std::pow(2, k - m)));
    }

    return ret;
}
}  // namespace qmpc::Share

using Share = qmpc::Share::Share<FixedPoint>;
using ShareComp = qmpc::Share::Share<PrimeField>;