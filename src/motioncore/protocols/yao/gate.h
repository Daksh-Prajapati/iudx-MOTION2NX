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

#include <cstdint>
#include <variant>

#include "gate/new_gate.h"
#include "utility/bit_vector.h"
#include "utility/block.h"
#include "utility/reusable_future.h"
#include "wire.h"

namespace ENCRYPTO::ObliviousTransfer {
class FixedXCOT128Receiver;
class FixedXCOT128Sender;
class GOT128Receiver;
class GOT128Sender;
}  // namespace ENCRYPTO::ObliviousTransfer

namespace MOTION::proto::yao {

class YaoProvider;
class YaoWire;
using YaoWireVector = std::vector<std::shared_ptr<YaoWire>>;

enum class OutputRecipient : std::uint8_t;

namespace detail {

class BasicYaoInputGate : public NewGate {
 public:
  BasicYaoInputGate(std::size_t gate_id, YaoProvider&, std::size_t num_wires, std::size_t num_simd);
  BasicYaoInputGate(std::size_t gate_id, YaoProvider&, std::size_t num_wires, std::size_t num_simd,
                    ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>>&&);
  YaoWireVector& get_output_wires() noexcept { return outputs_; };

 protected:
  YaoProvider& yao_provider_;
  const std::size_t num_wires_;
  const std::size_t num_simd_;
  ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>> input_future_;
  YaoWireVector outputs_;
};

class BasicYaoOutputGate : public NewGate {
 public:
  BasicYaoOutputGate(std::size_t gate_id, YaoProvider&, YaoWireVector&&, OutputRecipient);
  virtual ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>> get_output_future() = 0;

 protected:
  YaoProvider& yao_provider_;
  std::size_t num_wires_;
  ENCRYPTO::ReusableFiberPromise<std::vector<ENCRYPTO::BitVector<>>> output_promise_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::BitVector<>> bits_future_;
  ENCRYPTO::BitVector<> decoding_info_;
  const YaoWireVector inputs_;
  OutputRecipient output_recipient_;
};

class BasicYaoUnaryGate : public NewGate {
 public:
  BasicYaoUnaryGate(std::size_t gate_id, YaoProvider&, YaoWireVector&&, bool forward);
  YaoWireVector& get_output_wires() noexcept { return outputs_; };

 protected:
  YaoProvider& yao_provider_;
  std::size_t num_wires_;
  const YaoWireVector inputs_;
  YaoWireVector outputs_;
};

class BasicYaoBinaryGate : public NewGate {
 public:
  BasicYaoBinaryGate(std::size_t gate_id, YaoProvider&, YaoWireVector&&, YaoWireVector&&);
  YaoWireVector& get_output_wires() noexcept { return outputs_; };

 protected:
  YaoProvider& yao_provider_;
  std::size_t num_wires_;
  const YaoWireVector inputs_a_;
  const YaoWireVector inputs_b_;
  YaoWireVector outputs_;
};

}  // namespace detail

class YaoInputGateGarbler : public detail::BasicYaoInputGate {
 public:
  YaoInputGateGarbler(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                      std::size_t num_simd, bool in_setup);
  YaoInputGateGarbler(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                      std::size_t num_simd, bool in_setup,
                      ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>>&&);
  // create with in_setup = false
  YaoInputGateGarbler(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                      std::size_t num_simd);
  YaoInputGateGarbler(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                      std::size_t num_simd,
                      ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>>&&);

  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;

 private:
  std::variant<bool, std::shared_ptr<ENCRYPTO::ObliviousTransfer::GOT128Sender>,
               std::shared_ptr<ENCRYPTO::ObliviousTransfer::FixedXCOT128Sender>>
      ot_sender_;
};

class YaoInputGateEvaluator : public detail::BasicYaoInputGate {
 public:
  YaoInputGateEvaluator(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                        std::size_t num_simd, bool in_setup);
  YaoInputGateEvaluator(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                        std::size_t num_simd, bool in_setup,
                        ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>>&&);
  // create with in_setup = false
  YaoInputGateEvaluator(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                        std::size_t num_simd);
  YaoInputGateEvaluator(std::size_t gate_id, YaoProvider&, std::size_t num_wires,
                        std::size_t num_simd,
                        ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>>&&);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;

 private:
  std::variant<bool, std::shared_ptr<ENCRYPTO::ObliviousTransfer::GOT128Receiver>,
               std::shared_ptr<ENCRYPTO::ObliviousTransfer::FixedXCOT128Receiver>>
      ot_receiver_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> keys_future_;
};

class YaoOutputGateGarbler : public detail::BasicYaoOutputGate {
 public:
  YaoOutputGateGarbler(std::size_t gate_id, YaoProvider& yao_provider, YaoWireVector&& in,
                       OutputRecipient);
  ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>> get_output_future() override;
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_setup_wo_broadcast() override;
  void evaluate_online() override;
  // void evaluate_online_wo_output() override;
};

class YaoOutputGateEvaluator : public detail::BasicYaoOutputGate {
 public:
  YaoOutputGateEvaluator(std::size_t gate_id, YaoProvider& yao_provider, YaoWireVector&& in,
                         OutputRecipient);
  ENCRYPTO::ReusableFiberFuture<std::vector<ENCRYPTO::BitVector<>>> get_output_future() override;
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
};

class YaoINVGateGarbler : public detail::BasicYaoUnaryGate {
 public:
  YaoINVGateGarbler(std::size_t gate_id, YaoProvider&, YaoWireVector&&);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
};

class YaoINVGateEvaluator : public detail::BasicYaoUnaryGate {
 public:
  YaoINVGateEvaluator(std::size_t gate_id, YaoProvider&, YaoWireVector&&);
  bool need_setup() const noexcept override { return false; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override {}
  void evaluate_online() override {}
};

class YaoXORGateGarbler : public detail::BasicYaoBinaryGate {
 public:
  YaoXORGateGarbler(std::size_t gate_id, YaoProvider&, YaoWireVector&&, YaoWireVector&&);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
};

class YaoXORGateEvaluator : public detail::BasicYaoBinaryGate {
 public:
  YaoXORGateEvaluator(std::size_t gate_id, YaoProvider&, YaoWireVector&&, YaoWireVector&&);
  bool need_setup() const noexcept override { return false; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override {}
  void evaluate_online() override;
};

class YaoANDGateGarbler : public detail::BasicYaoBinaryGate {
 public:
  YaoANDGateGarbler(std::size_t gate_id, YaoProvider&, YaoWireVector&&, YaoWireVector&&);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
};

class YaoANDGateEvaluator : public detail::BasicYaoBinaryGate {
 public:
  YaoANDGateEvaluator(std::size_t gate_id, YaoProvider&, YaoWireVector&&, YaoWireVector&&);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;

 private:
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_fut_;
};

}  // namespace MOTION::proto::yao
