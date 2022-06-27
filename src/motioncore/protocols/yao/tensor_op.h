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

#include "gate/new_gate.h"
#include "protocols/beavy/tensor.h"
#include "protocols/gmw/tensor.h"
#include "tensor.h"
#include "tensor/tensor_op.h"
#include "utility/bit_vector.h"
#include "utility/reusable_future.h"

namespace ENCRYPTO {
struct AlgorithmDescription;
namespace ObliviousTransfer {
class FixedXCOT128Receiver;
class FixedXCOT128Sender;
class GOT128Receiver;
class GOT128Sender;
}  // namespace ObliviousTransfer
}  // namespace ENCRYPTO

namespace MOTION::proto::yao {

class YaoProvider;

template <typename T>
class ArithmeticGMWToYaoTensorConversionGarbler : public NewGate {
 public:
  ArithmeticGMWToYaoTensorConversionGarbler(std::size_t gate_id, YaoProvider&,
                                            const gmw::ArithmeticGMWTensorCP<T> input);
  ~ArithmeticGMWToYaoTensorConversionGarbler();
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const gmw::ArithmeticGMWTensorCP<T> input_;
  YaoTensorP output_;
  std::unique_ptr<ENCRYPTO::ObliviousTransfer::GOT128Sender> ot_sender_;
  ENCRYPTO::block128_vector garbler_input_keys_;
  ENCRYPTO::block128_vector evaluator_input_keys_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

template <typename T>
class ArithmeticGMWToYaoTensorConversionEvaluator : public NewGate {
 public:
  ArithmeticGMWToYaoTensorConversionEvaluator(std::size_t gate_id, YaoProvider&,
                                              const gmw::ArithmeticGMWTensorCP<T> input);
  ~ArithmeticGMWToYaoTensorConversionEvaluator();
  bool need_setup() const noexcept override { return false; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override {}
  void evaluate_online() override;
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const gmw::ArithmeticGMWTensorCP<T> input_;
  YaoTensorP output_;
  std::unique_ptr<ENCRYPTO::ObliviousTransfer::GOT128Receiver> ot_receiver_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbler_input_keys_future_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_future_;
  ENCRYPTO::block128_vector garbler_input_keys_;
  ENCRYPTO::block128_vector evaluator_input_keys_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

template <typename T>
class YaoToArithmeticGMWTensorConversionGarbler : public NewGate {
 public:
  YaoToArithmeticGMWTensorConversionGarbler(std::size_t gate_id, YaoProvider&,
                                            const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
  gmw::ArithmeticGMWTensorCP<T> get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  gmw::ArithmeticGMWTensorP<T> output_;
  ENCRYPTO::block128_vector mask_input_keys_;
  ENCRYPTO::block128_vector output_keys_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

template <typename T>
class YaoToArithmeticGMWTensorConversionEvaluator : public NewGate {
 public:
  YaoToArithmeticGMWTensorConversionEvaluator(std::size_t gate_id, YaoProvider&,
                                              const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  gmw::ArithmeticGMWTensorCP<T> get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  gmw::ArithmeticGMWTensorP<T> output_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbler_input_keys_future_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_future_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::BitVector<>> output_info_future_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

class YaoToBooleanGMWTensorConversionGarbler : public NewGate {
 public:
  YaoToBooleanGMWTensorConversionGarbler(std::size_t gate_id, YaoProvider&,
                                            const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
  gmw::BooleanGMWTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  gmw::BooleanGMWTensorP output_;
};

class YaoToBooleanGMWTensorConversionEvaluator : public NewGate {
 public:
  YaoToBooleanGMWTensorConversionEvaluator(std::size_t gate_id, YaoProvider&,
                                              const YaoTensorCP input);
  bool need_setup() const noexcept override { return false; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override {}
  void evaluate_online() override;
  gmw::BooleanGMWTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  gmw::BooleanGMWTensorP output_;
};

template <typename T>
class ArithmeticBEAVYToYaoTensorConversionGarbler : public NewGate {
 public:
  ArithmeticBEAVYToYaoTensorConversionGarbler(std::size_t gate_id, YaoProvider&,
                                              const beavy::ArithmeticBEAVYTensorCP<T> input);
  ~ArithmeticBEAVYToYaoTensorConversionGarbler();
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const beavy::ArithmeticBEAVYTensorCP<T> input_;
  YaoTensorP output_;
  std::unique_ptr<ENCRYPTO::ObliviousTransfer::FixedXCOT128Sender> ot_sender_;
  ENCRYPTO::block128_vector garbler_input_keys_;
  ENCRYPTO::block128_vector evaluator_input_keys_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

template <typename T>
class ArithmeticBEAVYToYaoTensorConversionEvaluator : public NewGate {
 public:
  ArithmeticBEAVYToYaoTensorConversionEvaluator(std::size_t gate_id, YaoProvider&,
                                                const beavy::ArithmeticBEAVYTensorCP<T> input);
  ~ArithmeticBEAVYToYaoTensorConversionEvaluator();
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const beavy::ArithmeticBEAVYTensorCP<T> input_;
  YaoTensorP output_;
  std::unique_ptr<ENCRYPTO::ObliviousTransfer::FixedXCOT128Receiver> ot_receiver_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbler_input_keys_future_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_future_;
  ENCRYPTO::block128_vector garbler_input_keys_;
  ENCRYPTO::block128_vector evaluator_input_keys_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

template <typename T>
class YaoToArithmeticBEAVYTensorConversionGarbler : public NewGate {
 public:
  YaoToArithmeticBEAVYTensorConversionGarbler(std::size_t gate_id, YaoProvider&,
                                              const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  beavy::ArithmeticBEAVYTensorCP<T> get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  beavy::ArithmeticBEAVYTensorP<T> output_;
  ENCRYPTO::block128_vector mask_input_keys_;
  ENCRYPTO::block128_vector output_keys_;
  ENCRYPTO::block128_vector garbled_tables_;
  ENCRYPTO::ReusableFiberFuture<std::vector<T>> masked_value_public_share_future_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

template <typename T>
class YaoToArithmeticBEAVYTensorConversionEvaluator : public NewGate {
 public:
  YaoToArithmeticBEAVYTensorConversionEvaluator(std::size_t gate_id, YaoProvider&,
                                                const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  beavy::ArithmeticBEAVYTensorCP<T> get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  static constexpr auto bit_size_ = ENCRYPTO::bit_size_v<T>;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  beavy::ArithmeticBEAVYTensorP<T> output_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbler_input_keys_future_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_future_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::BitVector<>> output_info_future_;
  std::vector<T> masked_value_secret_share_;
  std::vector<T> masked_value_public_share_;
  const ENCRYPTO::AlgorithmDescription& addition_algo_;
};

class YaoToBooleanBEAVYTensorConversionGarbler : public NewGate {
 public:
  YaoToBooleanBEAVYTensorConversionGarbler(std::size_t gate_id, YaoProvider&,
                                            const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  beavy::BooleanBEAVYTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  beavy::BooleanBEAVYTensorP output_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::BitVector<>> public_share_future_;
};

class YaoToBooleanBEAVYTensorConversionEvaluator : public NewGate {
 public:
  YaoToBooleanBEAVYTensorConversionEvaluator(std::size_t gate_id, YaoProvider&,
                                              const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override;
  void evaluate_online() override;
  beavy::BooleanBEAVYTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  beavy::BooleanBEAVYTensorP output_;
};

class YaoTensorReluGarbler : public NewGate {
 public:
  YaoTensorReluGarbler(std::size_t gate_id, YaoProvider&, const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  const YaoTensorP output_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& relu_algo_;
};

class YaoTensorReluEvaluator : public NewGate {
 public:
  YaoTensorReluEvaluator(std::size_t gate_id, YaoProvider&, const YaoTensorCP input);
  bool need_setup() const noexcept override { return false; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override {}
  void evaluate_online() override;
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  const YaoTensorP output_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_future_;
  const ENCRYPTO::AlgorithmDescription& relu_algo_;
};

class YaoTensorMaxPoolGarbler : public NewGate {
 public:
  YaoTensorMaxPoolGarbler(std::size_t gate_id, YaoProvider&, tensor::MaxPoolOp,
                          const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const tensor::MaxPoolOp maxpool_op_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  const YaoTensorP output_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& maxpool_algo_;
};

class YaoTensorMaxPoolEvaluator : public NewGate {
 public:
  YaoTensorMaxPoolEvaluator(std::size_t gate_id, YaoProvider&, tensor::MaxPoolOp,
                            const YaoTensorCP input);
  bool need_setup() const noexcept override { return false; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override {}
  void evaluate_online() override;
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const tensor::MaxPoolOp maxpool_op_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  const YaoTensorP output_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_future_;
  const ENCRYPTO::AlgorithmDescription& maxpool_algo_;
};

class YaoTensorGTGarbler : public NewGate {
 public:
  YaoTensorGTGarbler(std::size_t gate_id, YaoProvider&, tensor::MaxPoolOp,
                          const YaoTensorCP input);
  bool need_setup() const noexcept override { return true; }
  bool need_online() const noexcept override { return false; }
  void evaluate_setup() override;
  void evaluate_online() override {}
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const tensor::MaxPoolOp maxpool_op_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  const YaoTensorP output_;
  ENCRYPTO::block128_vector garbled_tables_;
  const ENCRYPTO::AlgorithmDescription& maxpool_algo_;
};

class YaoTensorGTEvaluator : public NewGate {
 public:
  YaoTensorGTEvaluator(std::size_t gate_id, YaoProvider&, tensor::MaxPoolOp,
                            const YaoTensorCP input);
  bool need_setup() const noexcept override { return false; }
  bool need_online() const noexcept override { return true; }
  void evaluate_setup() override {}
  void evaluate_online() override;
  YaoTensorCP get_output_tensor() const noexcept { return output_; }

 private:
  YaoProvider& yao_provider_;
  const tensor::MaxPoolOp maxpool_op_;
  const std::size_t bit_size_;
  const std::size_t data_size_;
  const YaoTensorCP input_;
  const YaoTensorP output_;
  ENCRYPTO::ReusableFiberFuture<ENCRYPTO::block128_vector> garbled_tables_future_;
  const ENCRYPTO::AlgorithmDescription& maxpool_algo_;
};

}  // namespace MOTION::proto::yao
