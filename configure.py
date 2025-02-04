#!/usr/bin/env python3
import glob
import io
import os
from vendor.ninja_syntax import Writer


# TODO: Debug?
RELEASE_MWCC_FLAGS = [
    # System
    "-nodefaults",
    "-nosyspath",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-enc SJIS",
    "-fp hardware",
    "-Cpp_exceptions off",
    '-pragma "cats off"',
    "-ipa file",
    "-opt all",
    "-inline auto",
    # User
    "-i include",
]

# TODO: Debug?
RELEASE_MWLD_FLAGS = [
    "-fp hard",
    "-nodefaults",
    "-mapunused",
    "-listclosure",
    "-lcf " + os.path.join("$builddir", "ldscript.lcf"),
]

out_buf = io.StringIO()
n = Writer(out_buf)

n.variable("ninja_required_version", "1.3")
n.newline()

n.variable("builddir", "build")
n.variable("outdir", "out")
n.newline()

# TODO: The non-Windows people aren't gonna be happy about this one
# NOTE: Perhaps DDD has the answer to this
n.variable("compiler", os.path.join("$builddir", "compiler", "mwcceppc.exe"))
n.variable("linker", os.path.join("$builddir", "compiler", "mwldeppc.exe"))
n.variable("dtk", os.path.join("$builddir", "dtk.exe"))
n.newline()

n.rule(
    "mwcc",
    command="$compiler $cflags -MMD -c $in -o $basedir",
    description="MWCC $out",
    depfile="$out.d",
    deps="gcc",
)
n.rule(
    "mwld",
    command="$linker $ldflags -o $out @$out.rsp",
    description="MWLD $out",
    rspfile="$out.rsp",
    rspfile_content="$in_newline",
)
n.rule("dol", command="$dtk elf2dol $in $out", description="DOL $out")

# TODO: Fix dependencies on both "src" and "cpp"
code_in_files = [
    *sorted(glob.glob("src/**/*.c", recursive=True)),
    *sorted(glob.glob("src/**/*.cpp", recursive=True)),
]
code_out_files = []

for in_file in code_in_files:
    code_out_file = os.path.join("$builddir", os.path.splitext(in_file)[0] + ".o")
    code_out_files.append(code_out_file)

    n.build(
        outputs=code_out_file,
        rule="mwcc",
        inputs=in_file,
        variables={
            "cflags": " ".join(RELEASE_MWCC_FLAGS),
            "basedir": os.path.join("$builddir", os.path.dirname(in_file)),
        },
    )

n.build(
    outputs=os.path.join("$outdir", "main.elf"),
    rule="mwld",
    inputs=code_out_files,
    variables={"ldflags": " ".join(RELEASE_MWLD_FLAGS)},
)

n.build(
    outputs=os.path.join("$outdir", "main.dol"),
    rule="dol",
    inputs=os.path.join("$outdir", "main.elf"),
)

with open("build.ninja", "w") as out_file:
    out_file.write(out_buf.getvalue())
n.close()
