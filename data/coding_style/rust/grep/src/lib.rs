use anyhow::Context;
use anyhow::Error;

use std::fs::File;
use std::io::Read;
use structopt::clap::AppSettings;
use structopt::StructOpt;

/// While using `&[&str]` to handle flags is convenient for exercise purposes,
/// and resembles the output of [`std::env::args`], in real-world projects it is
/// both more convenient and more idiomatic to contain runtime configuration in
/// a dedicated struct. Therefore, we suggest that you do so in this exercise.
///
/// In the real world, it's common to use crates such as [`clap`] or
/// [`structopt`] to handle argument parsing, and of course doing so is
/// permitted in this exercise as well, though it may be somewhat overkill.
///
/// [`clap`]: https://crates.io/crates/clap
/// [`std::env::args`]: https://doc.rust-lang.org/std/env/fn.args.html
/// [`structopt`]: https://crates.io/crates/structopt

#[derive(StructOpt, Debug)]
#[structopt(name = "Grep", setting = AppSettings::NoBinaryName)]
pub struct Flags {
    #[structopt(short, long)]
    /// Print the line numbers of each matching line.
    n: bool,
    #[structopt(short, long)]
    /// Print only the names of files that contain at least one matching line.
    l: bool,

    /// Match line using a case-insensitive comparison.
    #[structopt(short, long)]
    i: bool,

    /// Invert the program -- collect all lines that fail to match the pattern.
    #[structopt(short, long)]
    v: bool,
    /// Only match entire lines, instead of lines that contain a match.
    #[structopt(short, long)]
    x: bool,
}

impl Flags {
    pub fn new(flags: &[&str]) -> Self {
        Flags::from_iter(flags.iter())
    }
}

pub fn grep(pattern: &str, flags: &Flags, files: &[&str]) -> Result<Vec<String>, Error> {
    let mut res: Vec<String> = Vec::new();

    let line_numbers = if flags.n {
        |i: usize| {
            format! {"{}:",i}
        }
    } else {
        |_| "".into()
    };

    let ignore_case = if flags.i {
        |s: &str| s.to_lowercase()
    } else {
        |s: &str| s.into()
    };

    let matcher = if flags.x {
        |line: &str, pattern: &str| line.eq(pattern)
    } else {
        |line: &str, pattern: &str| line.contains(pattern)
    };

    let prepend_filename = if files.len() > 1 {
        |file: String| file + ":"
    } else {
        |_| "".into()
    };

    for file in files {
        let mut data = String::new();
        let mut f = File::open(file).with_context(|| format!("Failed to open file {}", file))?;
        f.read_to_string(&mut data)
            .context("Failed to read from file into buffer")?;
        for (i, l) in data.lines().enumerate() {
            if matcher(&ignore_case(l), &ignore_case(pattern)) != flags.v {
                if flags.l {
                    res.push(file.to_string());
                    break;
                } else {
                    res.push(prepend_filename(file.to_string()) + &line_numbers(i + 1) + l);
                };
            }
        }
    }

    Ok(res)
}
