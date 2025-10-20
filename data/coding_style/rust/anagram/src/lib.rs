use std::collections::HashSet;
use unicode_segmentation::UnicodeSegmentation;

pub fn anagrams_for<'a>(word: &str, possible_anagrams: &[&'a str]) -> HashSet<&'a str> {
    let mut res = HashSet::new();
    let word_gr = UnicodeSegmentation::graphemes(word, true)
        .map(|s| s.to_lowercase())
        .collect::<Vec<String>>();
    let mut word_gr_sorted = word_gr.clone();
    word_gr_sorted.sort_unstable();

    for pa in possible_anagrams {
        let mut pa_gr = UnicodeSegmentation::graphemes(*pa, true)
            .map(|s| s.to_lowercase())
            .collect::<Vec<String>>();

        if word_gr != pa_gr {
            pa_gr.sort_unstable();
            if word_gr_sorted == pa_gr {
                res.insert(*pa);
            }
        }
    }

    res
}
