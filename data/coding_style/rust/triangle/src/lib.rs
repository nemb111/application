use num::Num;

pub struct Triangle<T> {
    sides: [T; 3],
}

impl<T> Triangle<T>
where
    T: Num + PartialOrd + Copy,
{
    pub fn build(mut sides: [T; 3]) -> Option<Triangle<T>> {
        // unimplemented!("Construct new Triangle from following sides: {:?}. Return None if the sides are invalid.", sides);
        sides.sort_by(|a, b| a.partial_cmp(b).unwrap());
        if sides[0] + sides[1] <= sides[2] || sides.iter().any(|&x| x.is_zero()) {
            return None;
        }

        Some(Triangle { sides })
    }

    pub fn is_equilateral(&self) -> bool {
        // unimplemented!("Determine if the Triangle is equilateral.");
        let mut res: Vec<&T> = self.sides.iter().collect();
        res.sort_by(|a, b| a.partial_cmp(b).unwrap());;
        res.dedup();
        1 == res.len()
    }

    pub fn is_scalene(&self) -> bool {
        // unimplemented!("Determine if the Triangle is scalene.");
        let mut res: Vec<&T> = self.sides.iter().collect();
        res.sort_by(|a, b| a.partial_cmp(b).unwrap());;
        res.dedup();
        3 == res.len()
    }

    pub fn is_isosceles(&self) -> bool {
        // unimplemented!("Determine if the Triangle is isosceles.");
        let mut res: Vec<&T> = self.sides.iter().collect();
        res.sort_by(|a, b| a.partial_cmp(b).unwrap());;
        res.dedup();
        2 == res.len()
    }
}
