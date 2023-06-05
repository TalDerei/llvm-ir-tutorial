// LLVM Pass for LLVM 14
// Run: 'clang -flegacy-pass-manager -Xclang -load -Xclang build/skeleton/libSkeletonPass.* llvm.c'

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

// LLVM function pass
namespace {
    struct FunctionNamePass : public FunctionPass {
        static char ID;
        FunctionNamePass() : FunctionPass(ID) {}

        virtual bool runOnFunction(Function &F) {
            errs() << "[*] function '" << F.getName() << "'\n";

            // Iterate over basic blocks
            for (auto &B : F) {
                // Iterate over instructions
                for (auto &I : B) {
                    if (auto* op = dyn_cast<BinaryOperator>(&I)) { // check if binary operator
                        errs() << "  [+] binary operator: " << op->getOpcodeName() << "\n";
                        // Uniform API for creating instructions and inserting them into a basic block,
                        // in this case where the 'op' instruction appears
                        IRBuilder<> builder(op);

                        // Make a multiply with the same operands as 'op'
                        Value *lhs = op->getOperand(0);
                        Value *rhs = op->getOperand(1);
                        Value *mul = builder.CreateMul(lhs, rhs);

                        // Swap out old instruction for new instruction for every instance of 'op'
                        for (auto& U : op->uses()) {                        
                            User *user = U.getUser();
                            user->setOperand(U.getOperandNo(), mul);
                        }
                    }
                }
            }
            // We modified the code 
            return true;
        }
    };
}

char FunctionNamePass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerFunctionNamePass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
    PM.add(new FunctionNamePass());
}
static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible, registerFunctionNamePass);
