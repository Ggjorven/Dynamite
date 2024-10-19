import os
import sys
import glob
from subprocess import run, PIPE

# Options
s_NeedSudo: bool = False
s_NativeSuffix: bool = False

# Note: example input: /usr/lib/**.a or /usr/lib/randomlib.a
def GetAllFiles(input: str) -> list[str]:
    # fullFileName: str = os.path.basename(input)
    # fileName: str = fullFileName.split('.')[0]
    # extension: str = fullFileName.split('.')[1]
    # dir: str = os.path.dirname(input)
    
    return glob.glob(input, recursive=True)

def RunCommand(args: tuple[str]) -> str:
    result = run(args, stdout=PIPE, stderr=PIPE)

    if result.returncode != 0:
        print(f"Error running command: {result.stderr.decode()}")
        return ""

    return result.stdout.decode().rstrip('\n')

def ConvertFile(file: str) -> None:
    print(f"Converting LLVM lib \"{file}\" to regular lib file.")

    # Get the object filename
    objectFilenames = RunCommand(["ar", "-t", file]) # No type, we want to use pythons dynamic type system next.
    objectFilenames: list[str] = objectFilenames.split('\n')

    # Output the object file to disk
    RunCommand(["ar", "-x", file])

    # Convert LLVM IR object files to native object files
    for obj in objectFilenames:
        if obj: # Check if the obj is valid
            result = RunCommand(["file", obj])

            # Note: Only make it native if it is LLVM IR bitcode 
            if "LLVM IR bitcode" in result:
                RunCommand(["llc", "-filetype=obj", obj, "-o", obj])

    # Create a new native library
    libName = ""
    if s_NativeSuffix:
        libName: str = f"{file.split('.')[0]}_native.a"
    else:
        libName: str = f"{file.split('.')[0]}.a"

    RunCommand(["ar", "rcs", os.path.basename(libName), *objectFilenames])

    # Delete temporary object file from disk
    for obj in objectFilenames:
        if obj: # Check if the obj is valid
            RunCommand(["rm", obj])



def main(argv: list[str]) -> int:
    if s_NeedSudo:
        RunCommand(["sudo", "su"])

    for i in range(1, len(argv)):
        for f in GetAllFiles(argv[i]):
            print(f"Retrieved filename: {f}")
            ConvertFile(f)

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))