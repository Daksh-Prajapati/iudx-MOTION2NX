// MIT License
//
// Copyright (c) 2020 Lennart Braun
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

#include <functional>
#include <memory>

namespace MOTION {

class Logger;
class GateRegister;

namespace Statistics {
struct RunTimeStats;
}

// Evaluates all registered gates.
class NewGateExecutor {
 public:
  NewGateExecutor(GateRegister&, std::function<void()> preprocessing_fctn,
                  bool sync_between_setup_and_online, std::function<void()> sync_fctn,
                  std::size_t num_threads, std::shared_ptr<Logger>);
  NewGateExecutor(GateRegister&, std::function<void()> preprocessing_fctn, std::size_t num_threads,
                  std::shared_ptr<Logger>);

  // Run the setup phases first for all gates before starting with the online
  // phases.
  void evaluate_setup_online(Statistics::RunTimeStats& stats);
  void evaluate_setup_online_wo_broadcast(Statistics::RunTimeStats& stats);
  // Run setup and online phase of each gate as soon as possible.
  void evaluate(Statistics::RunTimeStats& stats);

 private:
  void evaluate_setup_online_multi_threaded(Statistics::RunTimeStats& stats);
  void evaluate_setup_online_single_threaded(Statistics::RunTimeStats& stats);
  void evaluate_setup_online_wo_broadcast_multi_threaded(Statistics::RunTimeStats& stats);

  GateRegister& register_;
  std::function<void()> preprocessing_fctn_;
  std::function<void()> sync_fctn_;
  std::size_t num_threads_;
  bool sync_between_setup_and_online_ = false;
  std::shared_ptr<Logger> logger_;
};

}  // namespace MOTION
