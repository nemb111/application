// 3 bottles of beer on the wall, 3 bottles of beer.
// Take one down and pass it around, 2 bottles of beer on the wall.

// 2 bottles of beer on the wall, 2 bottles of beer.
// Take one down and pass it around, 1 bottle of beer on the wall.

// 1 bottle of beer on the wall, 1 bottle of beer.
// Take it down and pass it around, no more bottles of beer on the wall.

// No more bottles of beer on the wall, no more bottles of beer.
// Go to the store and buy some more, 99 bottles of beer on the wall.

static VERSE_STRINGS: [&str; 3] = [
    "No more bottles of beer on the wall, no more bottles of beer.
Go to the store and buy some more, 99 bottles of beer on the wall.\n",
    "1 bottle of beer on the wall, 1 bottle of beer.
Take it down and pass it around, no more bottles of beer on the wall.\n",
"2 bottles of beer on the wall, 2 bottles of beer.\nTake one down and pass it around, 1 bottle of beer on the wall.\n",
];

pub fn verse(n: u32) -> String {
    match n {
        // 0 => format!(VERSE_STRINGS[0]),
        0 => VERSE_STRINGS[0].to_string(),
        1 => VERSE_STRINGS[1].to_string(),
        2 => VERSE_STRINGS[2].to_string(),
        _ => format!(
            "{} bottles of beer on the wall, {} bottles of beer.
Take one down and pass it around, {} bottles of beer on the wall.\n",
            n,
            n,
            n - 1
        ),
    }
}

pub fn sing(start: u32, end: u32) -> String {
    let mut res = String::new();

    for i in (end..=start).rev() {
        res.push_str(&verse(i));
        if i != end {
            res.push_str("\n");
        }
    }

    res
}
