struct SplitStrByConsonantCluster {
    consonants_cluster: String,
    cluster_suffix_str: String,
}

impl SplitStrByConsonantCluster {
    fn new(input: &str) -> Self {
        let consonants_cluster: String = input
            .chars()
            .take_while(|c| !['a', 'e', 'i', 'o', 'u'].contains(c))
            .collect();

        let cluster_suffix_str = input
            .strip_prefix(&consonants_cluster)
            .unwrap_or("")
            .to_string();

        Self {
            consonants_cluster,
            cluster_suffix_str,
        }
    }

    pub fn get(&self) -> (&str, &str) {
        (&self.consonants_cluster, &self.cluster_suffix_str)
    }
}

fn rule1(input: &str) -> Option<String> {
    if input.starts_with('a')
        || input.starts_with('e')
        || input.starts_with('o')
        || input.starts_with('u')
        || input.starts_with('i')
        || input.starts_with("xr")
        || input.starts_with("yt")
    {
        return Some(format!("{}ay", input));
    }

    None
}

fn rule2((consonants_cluster, cluster_suffix_str): (&str, &str)) -> Option<String> {
    Some(cluster_suffix_str.to_string() + consonants_cluster + "ay")
}

fn rule3((consonants_cluster, cluster_suffix_str): (&str, &str)) -> Option<String> {
    if consonants_cluster.ends_with('q') && cluster_suffix_str.starts_with('u') {
        return cluster_suffix_str
            .strip_prefix('u')
            .map(|s| s.to_string() + consonants_cluster + "uay");
    }

    None
}

fn rule4((consonants_cluster, cluster_suffix_str): (&str, &str)) -> Option<String> {
    if let Some(idx) = consonants_cluster[1..].find('y') {
        return Some(
            consonants_cluster[idx + 1..].to_string()
                + cluster_suffix_str
                + &consonants_cluster[..idx + 1]
                + "ay",
        );
    }
    None
}

pub fn translate(input: &str) -> String {
    let input = input.split_whitespace();
    let res: Vec<_> = input
        .into_iter()
        .map(|i| {
            if let Some(res) = rule1(i) {
                res
            } else {
                let split_str_by_con_cluster = SplitStrByConsonantCluster::new(i);
                let (con_cluster, cluster_suffix) = split_str_by_con_cluster.get();

                if let Some(res) = rule4((con_cluster, cluster_suffix)) {
                    return res;
                } else if let Some(res) = rule3((con_cluster, cluster_suffix)) {
                    return res;
                } else if let Some(res) = rule2((con_cluster, cluster_suffix)) {
                    return res;
                }

                "".to_string()
            }
        })
        .collect();
    res.join(" ")
}
