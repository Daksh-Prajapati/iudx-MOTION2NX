// MIT License
//
// Copyright (c) 2019 Lennart Braun
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/json/object.hpp>
#include <list>
#include "run_time_stats.h"

namespace MOTION {

namespace Communication {
struct TransportStatistics;
}

namespace Statistics {

struct AccumulatedRunTimeStats {
  using clock_type = std::chrono::steady_clock;
  using duration = clock_type::duration;
  using resolution = std::milli;
  using accumulator_type = boost::accumulators::accumulator_set<
      double,
      boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::median,
                                 boost::accumulators::tag::lazy_variance>>;

  std::size_t count_ = 0;
  std::array<accumulator_type, static_cast<std::size_t>(RunTimeStats::StatID::MAX) + 1>
      accumulators_;

  void add(const RunTimeStats& stats);
  std::string print_human_readable() const;
  boost::json::object to_json() const;
};

struct AccumulatedCommunicationStats {
  using accumulator_type = boost::accumulators::accumulator_set<
      std::size_t,
      boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::sum>>;

  static constexpr std::size_t idx_num_messages_sent = 0;
  static constexpr std::size_t idx_num_messages_received = 1;
  static constexpr std::size_t idx_num_bytes_sent = 2;
  static constexpr std::size_t idx_num_bytes_received = 3;

  std::size_t count_ = 0;
  std::array<accumulator_type, 4> accumulators_;

  void add(const Communication::TransportStatistics& stats);
  void add(const std::vector<Communication::TransportStatistics>& stats);
  std::string print_human_readable() const;
  boost::json::object to_json() const;
};

std::string print_stats(const std::string& experiment_name, const AccumulatedRunTimeStats&,
                        const AccumulatedCommunicationStats&);
std::string print_stats_short(const std::string& experiment_name, const AccumulatedRunTimeStats&,
                          const AccumulatedCommunicationStats&);
boost::json::object to_json(const std::string& experiment_name, const AccumulatedRunTimeStats&,
                            const AccumulatedCommunicationStats&);

}  // namespace Statistics
}  // namespace MOTION
