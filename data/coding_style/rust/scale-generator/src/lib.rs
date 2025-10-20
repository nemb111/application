// You should change this.
//
// Depending on your implementation, there are a variety of potential errors
// which might occur. They aren't checked by the test suite in order to
// allow the greatest freedom of implementation, but real libraries should
// provide useful, descriptive errors so that downstream code can react
// appropriately.
//
// One common idiom is to define an Error enum which wraps all potential
// errors. Another common idiom is to use a helper type such as failure::Error
// which does more or less the same thing but automatically.

pub type Error = ();

static MAJOR: [&str; 12] = [
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#",
];
static MINOR: [&str; 12] = [
    "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab",
];
static SHARPS: [&str; 14] = [
    "C", "a", "G", "D", "A", "E", "B", "F#", "e", "b", "f#", "c#", "g#", "d#",
];
static FLATS: [&str; 12] = [
    "F", "Bb", "Eb", "Ab", "Db", "Gb", "d", "g", "c", "f", "bb", "eb",
];

pub struct Scale<'a> {
    scale: Vec<&'a str>,
}

impl<'a> Scale<'a> {
    pub fn new(tonic: &'a str, intervals: &'a str) -> Result<Scale<'a>, Error> {
        let mut iter = match (
            SHARPS.iter().find(|&&x| x == tonic),
            FLATS.iter().find(|&&x| x == tonic),
        ) {
            (Some(_), None) => MAJOR.iter().cycle(),
            (None, Some(_)) => MINOR.iter().cycle(),
            _ => return Err(()),
        }
        .skip_while(|x| x.to_ascii_uppercase() != tonic.to_ascii_uppercase());

        let mut scale: Vec<_> = vec![*iter.by_ref().next().unwrap()];
        for i in intervals
            .chars()
            .take(intervals.len() - 1)
            .map(|c| match c {
                'm' => Ok(0),
                'M' => Ok(1),
                'A' => Ok(2),
                _ => Err(()),
            })
        {
            scale.push(*iter.by_ref().nth(i?).unwrap());
        }

        Ok(Scale { scale })
    }

    pub fn chromatic(tonic: &'a str) -> Result<Scale<'a>, Error> {
        Self::new(tonic, "mmmmmmmmmmmm")
    }

    pub fn enumerate(self) -> Vec<String> {
        self.scale.iter().map(|s| s.to_string()).collect()
    }
}
