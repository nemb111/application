pub mod graph {
    use self::graph_items::edge::Edge;
    use self::graph_items::node::Node;
    use std::collections::HashMap;

    #[derive(std::cmp::PartialEq, Debug, Default)]
    pub struct Graph {
        pub nodes: Vec<Node>,
        pub edges: Vec<Edge>,
        pub attrs: HashMap<String, String>,
    }

    impl Graph {
        pub fn new() -> Self {
            Default::default()
        }

        pub fn with_nodes(mut self, nodes: &[Node]) -> Graph {
            nodes.iter().for_each(|x| self.nodes.push(x.clone()));
            self
        }

        pub fn with_edges(mut self, edges: &[Edge]) -> Graph {
            edges.iter().for_each(|x| self.edges.push(x.clone()));
            self
        }

        pub fn with_attrs(mut self, attrs: &[(&str, &str)]) -> Graph {
            for (x, y) in attrs {
                self.attrs.insert(x.to_string(), y.to_string());
            }

            self
        }

        pub fn get_node(&self, label: &str) -> Option<&Node> {
            self.nodes.iter().find(|&x| x.label == label)
        }
    }

    pub mod graph_items {

        pub mod edge {
            use std::collections::HashMap;

            #[derive(PartialEq, Debug, Clone)]
            pub struct Edge {
                node1: String,
                node2: String,
                attrs: HashMap<String, String>,
            }

            impl Edge {
                pub fn new(node1: &str, node2: &str) -> Self {
                    Edge {
                        node1: node1.to_string(),
                        node2: node2.to_string(),
                        attrs: HashMap::new(),
                    }
                }

                pub fn with_attrs(mut self, attrs: &[(&str, &str)]) -> Self {
                    for (x, y) in attrs {
                        self.attrs.insert(x.to_string(), y.to_string());
                    }

                    self
                }
            }
        }

        pub mod node {
            use std::collections::HashMap;

            #[derive(PartialEq, Debug, Clone)]
            pub struct Node {
                pub label: String,
                attrs: HashMap<String, String>,
            }

            impl Node {
                pub fn new(label: &str) -> Self {
                    Node {
                        label: label.to_string(),
                        attrs: HashMap::new(),
                    }
                }

                pub fn with_attrs(mut self, attrs: &[(&str, &str)]) -> Self {
                    for (x, y) in attrs {
                        self.attrs.insert(x.to_string(), y.to_string());
                    }

                    self
                }

                pub fn get_attr(&self, attr_name: &str) -> Option<&str> {
                    match self.attrs.get(attr_name) {
                        Some(x) => Some(&x),
                        None => None,
                    }
                }
            }
        }
    }
}
