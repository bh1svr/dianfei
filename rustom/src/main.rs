use std::env;
use std::str::FromStr;
use std::fs::File;
use std::io::Read;

fn main() -> Result<(), std::io::Error> {
    let mut div = 10000;
    let mut args_usr = Vec::new();
    for arg in env::args().skip(1) {
        args_usr.push(u64::from_str(&arg).expect("Arg 1 not a number"));
    }
    if args_usr.len() == 1 {
        div = args_usr[0];
    }
    let mut r_file = File::open("/dev/random")?;
    let mut r_array = [0; 8];
    r_file.read(&mut r_array)?;
    let r = u64::from_le_bytes(r_array);
    let result = r % div;
    if args_usr.len() == 0 {
        println!("{}.{:02}", result / 100, result % 100);
    }
    else {
        println!("{}", result);
    }
    Ok(())
}

