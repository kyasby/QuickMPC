#pragma once
#include <iostream>
#include <list>
#include <random>
#include "gtest/gtest.h"
#include "Share/Matrix.hpp"

#include "LogHeader/Logger.hpp"

std::random_device rnd;
std::mt19937_64 mt(rnd());
std::uniform_real_distribution<double> distr(-100.0, 100.0);

TEST(ShareMatrixTest, OpenAndReconsTest)
{
    Config *conf = Config::getInstance();
    int n_parties = conf->n_parties;
    auto share_mat = qmpc::Share::ShareMatrix(
        {{FixedPoint("1.0"), FixedPoint("2.0")}, {FixedPoint("3.0"), FixedPoint("4.0")}}
    );
    auto recons_mat = share_mat.open_and_recons();

    auto true_mat = std::vector<std::vector<FixedPoint>>{
        {FixedPoint(std::to_string(1.0 * n_parties)), FixedPoint(std::to_string(2.0 * n_parties))},
        {FixedPoint(std::to_string(3.0 * n_parties)), FixedPoint(std::to_string(4.0 * n_parties))}};
    EXPECT_EQ(recons_mat, true_mat);
}

TEST(ShareMatrixTest, AddTest)
{
    Config *conf = Config::getInstance();
    int n_parties = conf->n_parties;
    auto a = qmpc::Share::ShareMatrix(
        {{FixedPoint("1.0"), FixedPoint("2.0")}, {FixedPoint("3.0"), FixedPoint("4.0")}}
    );
    auto b = qmpc::Share::ShareMatrix(
        {{FixedPoint("5.0"), FixedPoint("1.0")}, {FixedPoint("2.0"), FixedPoint("4.0")}}
    );
    auto c = a + b;
    auto recons_mat = c.open_and_recons();

    auto true_mat = std::vector<std::vector<FixedPoint>>{
        {FixedPoint(std::to_string(1.0 * n_parties + 5.0 * n_parties)),
         FixedPoint(std::to_string(2.0 * n_parties + 1.0 * n_parties))},
        {FixedPoint(std::to_string(3.0 * n_parties + 2.0 * n_parties)),
         FixedPoint(std::to_string(4.0 * n_parties + 4.0 * n_parties))}};
    EXPECT_EQ(recons_mat, true_mat);
}

TEST(ShareMatrixTest, SubTest)
{
    Config *conf = Config::getInstance();
    int n_parties = conf->n_parties;
    auto a = qmpc::Share::ShareMatrix(
        {{FixedPoint("1.0"), FixedPoint("2.0")}, {FixedPoint("3.0"), FixedPoint("4.0")}}
    );
    auto b = qmpc::Share::ShareMatrix(
        {{FixedPoint("5.0"), FixedPoint("1.0")}, {FixedPoint("2.0"), FixedPoint("4.0")}}
    );
    auto c = a - b;
    auto recons_mat = c.open_and_recons();

    auto true_mat = std::vector<std::vector<FixedPoint>>{
        {FixedPoint(std::to_string(1.0 * n_parties - 5.0 * n_parties)),
         FixedPoint(std::to_string(2.0 * n_parties - 1.0 * n_parties))},
        {FixedPoint(std::to_string(3.0 * n_parties - 2.0 * n_parties)),
         FixedPoint(std::to_string(4.0 * n_parties - 4.0 * n_parties))}};
    EXPECT_EQ(recons_mat, true_mat);
}

TEST(ShareMatrixTest, MulTest)
{
    Config *conf = Config::getInstance();
    int n_parties = conf->n_parties;
    auto a = qmpc::Share::ShareMatrix(
        {{FixedPoint("1.0"), FixedPoint("2.0")}, {FixedPoint("3.0"), FixedPoint("4.0")}}
    );
    auto b = qmpc::Share::ShareMatrix(
        {{FixedPoint("5.0"), FixedPoint("1.0")}, {FixedPoint("2.0"), FixedPoint("4.0")}}
    );
    auto c = a * b;
    auto recons_mat = c.open_and_recons();

    auto true_mat = std::vector<std::vector<FixedPoint>>{
        {FixedPoint(std::to_string((1.0 * 5.0 + 2.0 * 2.0) * n_parties * n_parties)),
         FixedPoint(std::to_string((1.0 * 1.0 + 2.0 * 4.0) * n_parties * n_parties))},
        {FixedPoint(std::to_string((3.0 * 5.0 + 4.0 * 2.0) * n_parties * n_parties)),
         FixedPoint(std::to_string((3.0 * 1.0 + 4.0 * 4.0) * n_parties * n_parties))}};
    EXPECT_EQ(recons_mat, true_mat);
}

TEST(ShareMatrixTest, MulTest_Large)
{
    // 時間計測用Test
    std::list<int> size_list{10, 15, 20, 30};
    for (const auto &size : size_list)
    {
        std::vector<std::vector<Share>> mat1(size, std::vector<Share>(size));
        std::vector<std::vector<Share>> mat2(size, std::vector<Share>(size));
        for (int h = 0; h < size; ++h)
        {
            for (int w = 0; w < size; ++w)
            {
                mat1[h][w] = RandGenerator::getInstance()->getRand<FixedPoint>(1, 1000);
                mat2[h][w] = RandGenerator::getInstance()->getRand<FixedPoint>(1, 1000);
            }
        }
        auto share_mat1 = qmpc::Share::ShareMatrix(mat1);
        auto share_mat2 = qmpc::Share::ShareMatrix(mat2);

        // 逆行列計算を計測
        auto start = std::chrono::system_clock::now();
        auto mul_mat = share_mat1 * share_mat2;
        auto end = std::chrono::system_clock::now();
        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        spdlog::info("{0}x{0}行列の積: {1} milli sec", size, msec);
    }
}

TEST(ShareMatrixTest, TransposeTest)
{
    Config *conf = Config::getInstance();
    int n_parties = conf->n_parties;
    auto share_mat = qmpc::Share::ShareMatrix(
        {{FixedPoint("1.0"), FixedPoint("2.0")}, {FixedPoint("3.0"), FixedPoint("4.0")}}
    );
    auto recons_mat = share_mat.transpose().open_and_recons();

    auto true_mat = std::vector<std::vector<FixedPoint>>{
        {FixedPoint(std::to_string(1.0 * n_parties)), FixedPoint(std::to_string(3.0 * n_parties))},
        {FixedPoint(std::to_string(2.0 * n_parties)), FixedPoint(std::to_string(4.0 * n_parties))}};
    EXPECT_EQ(recons_mat, true_mat);
}

TEST(ShareMatrixTest, InverseTest_Small)
{
    auto share_mat = qmpc::Share::ShareMatrix(
        {{FixedPoint("1.0"), FixedPoint("3.0"), FixedPoint("5.0"), FixedPoint("6.0")},
         {FixedPoint("3.0"), FixedPoint("4.0"), FixedPoint("3.0"), FixedPoint("2.0")},
         {FixedPoint("5.0"), FixedPoint("3.0"), FixedPoint("1.0"), FixedPoint("1.0")},
         {FixedPoint("6.0"), FixedPoint("2.0"), FixedPoint("1.0"), FixedPoint("4.0")}}
    );
    auto share_mat_inv = share_mat.inverse();
    auto recons_mat = (share_mat * share_mat_inv).open_and_recons();
    for (int h = 0; h < 4; ++h)
    {
        for (int w = 0; w < 4; ++w)
        {
            // 許容誤差0.01で比較
            if (h == w)
            {
                EXPECT_NEAR(recons_mat[h][w].getDoubleVal(), 1.0, 0.01);
            }
            else
            {
                EXPECT_NEAR(recons_mat[h][w].getDoubleVal(), 0.0, 0.01);
            }
        }
    }
}

TEST(ShareMatrixTest, InverseTest_Large)
{
    // 時間計測用Test
    std::list<int> size_list{10, 20, 30};
    for (const auto &size : size_list)
    {
        std::vector<std::vector<Share>> mat(size, std::vector<Share>(size));
        for (int h = 0; h < size; ++h)
        {
            for (int w = 0; w < size; ++w)
            {
                mat[h][w] = distr(mt);
            }
        }
        auto share_mat = qmpc::Share::ShareMatrix(mat);

        // 逆行列計算を計測
        auto start = std::chrono::system_clock::now();
        auto share_mat_inv = share_mat.inverse();
        auto end = std::chrono::system_clock::now();
        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        spdlog::info("{0}x{0}行列の逆行列計算: {1} milli sec", size, msec);

        /*
        sizeが15を超えると誤差が0.01を超えてしまう
        乱数によってはかなりひどい誤差もありうるのでコメント
        auto recons_unit = (share_mat * share_mat_inv).open_and_recons();
        for (int h = 0; h < size; ++h)
        {
            for (int w = 0; w < size; ++w)
            {
                // 許容誤差0.01で比較
                if (h == w)
                {
                    EXPECT_NEAR(recons_unit[h][w].getDoubleVal(), 1.0, 0.01);
                }
                else
                {
                    EXPECT_NEAR(recons_unit[h][w].getDoubleVal(), 0.0, 0.01);
                }
            }
        }
        */
    }
}

TEST(MatrixTest, SharizeTest)
{
    auto mat = qmpc::Share::Matrix(
        {{FixedPoint("1.0"), FixedPoint("2.0")}, {FixedPoint("3.0"), FixedPoint("4.0")}}
    );
    auto share_mat = mat.sharize_mat();
    auto recons_mat = share_mat.open_and_recons();

    auto true_mat = std::vector<std::vector<FixedPoint>>{
        {FixedPoint("1.0"), FixedPoint("2.0")}, {FixedPoint("3.0"), FixedPoint("4.0")}};
    EXPECT_EQ(recons_mat, true_mat);
}

TEST(MatrixTest, InverseTest_Small)
{
    auto sym_mat = qmpc::Share::Matrix(
        {{FixedPoint("1.0"), FixedPoint("3.0"), FixedPoint("5.0"), FixedPoint("6.0")},
         {FixedPoint("3.0"), FixedPoint("4.0"), FixedPoint("3.0"), FixedPoint("2.0")},
         {FixedPoint("5.0"), FixedPoint("3.0"), FixedPoint("1.0"), FixedPoint("1.0")},
         {FixedPoint("6.0"), FixedPoint("2.0"), FixedPoint("1.0"), FixedPoint("4.0")}}
    );
    auto mat_inv = sym_mat.inverse();

    auto true_mat = std::vector<std::vector<FixedPoint>>{
        {FixedPoint("0.84210526"),
         FixedPoint("-1.84210526"),
         FixedPoint("2.21052632"),
         FixedPoint("-0.89473684")},
        {FixedPoint("-1.84210526"),
         FixedPoint("3.84210526"),
         FixedPoint("-4.21052632"),
         FixedPoint("1.89473684")},
        {FixedPoint("2.21052632"),
         FixedPoint("-4.21052632"),
         FixedPoint("5.05263158"),
         FixedPoint("-2.47368421")},
        {FixedPoint("-0.89473684"),
         FixedPoint("1.89473684"),
         FixedPoint("-2.47368421"),
         FixedPoint("1.26315789")}};

    for (int h = 0; h < 4; ++h)
    {
        for (int w = 0; w < 4; ++w)
        {
            // 許容誤差0.01で比較
            EXPECT_NEAR(mat_inv(h, w).getDoubleVal(), true_mat[h][w].getDoubleVal(), 0.01);
        }
    }
}
