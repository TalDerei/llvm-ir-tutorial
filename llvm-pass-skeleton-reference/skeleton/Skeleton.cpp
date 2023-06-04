// LLVM Pass for LLVM 14
// Run: 'clang -flegacy-pass-manager -Xclang -load -Xclang build/skeleton/libSkeletonPass.* llvm.c'

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
    struct FunctionNamePass : public FunctionPass {
        static char ID;
        FunctionNamePass() : FunctionPass(ID) {}

        virtual bool runOnFunction(Function &F) {
            errs() << "[*] function '" << F.getName() << "'\n";
            return false;
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
