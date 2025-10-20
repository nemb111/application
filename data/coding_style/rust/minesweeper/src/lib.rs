struct IdsCalc {
    x_dim: usize,
    y_dim: usize,
}

impl IdsCalc {
    fn new(x_dim: usize, y_dim: usize) -> Self {
        IdsCalc { x_dim, y_dim }
    }

    fn calc_ids<'a>(
        &self,
        (x_idx, y_idx): (usize, usize),
        ids_arr: &'a mut [(usize, usize); 9],
    ) -> &'a [(usize, usize)] {
        use std::cmp;
        let mut idx = 0;
        for y in cmp::max(0, y_idx as i32 - 1) as usize..=cmp::min(y_idx + 1, self.y_dim - 1) {
            for x in cmp::max(0, x_idx as i32 - 1) as usize..=cmp::min(x_idx + 1, self.x_dim - 1) {
                if !(x == x_idx && y == y_idx) {
                    ids_arr[idx] = (x, y);
                    idx += 1;
                }
            }
        }

        &ids_arr[0..idx]
    }
}

pub fn annotate(minefield: &[&str]) -> Vec<String> {
    let y_dim = minefield.len();

    if y_dim == 0 {
        return vec![];
    }

    let x_dim = minefield[0].len();
    let ids_calc = IdsCalc::new(x_dim, y_dim);
    let mut arr = [(0_usize, 0_usize); 9];
    let mut res = Vec::with_capacity(y_dim);

    minefield.iter().for_each(|x| {
        res.push(x.to_string().into_bytes());
    });

    for y in minefield.iter().enumerate() {
        for x in y.1.chars().enumerate() {
            if x.1 == '*' {
                let ids_arr = ids_calc.calc_ids((x.0, y.0), &mut arr);
                println!("{:?}", ids_arr);

                for ids in ids_arr {
                    let el = res.get_mut(ids.1).unwrap().get_mut(ids.0).unwrap();
                    *el = match el {
                        b' ' => b'1',
                        b'*' => b'*',
                        _ => *el + 1,
                    }
                }
            }
        }
    }

    res.into_iter()
        .map(|x| String::from_utf8(x).expect("Found invalid UTF-8"))
        .collect::<Vec<String>>()
}
