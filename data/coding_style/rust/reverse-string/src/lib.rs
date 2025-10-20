use unicode_reverse::reverse_grapheme_clusters_in_place;

pub fn reverse(input: &str) -> String {
    let mut res_string = input.to_string();
    reverse_grapheme_clusters_in_place(&mut res_string);

    res_string
}
