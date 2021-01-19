
import React from 'react';
import ReactDOM from 'react-dom';
import _ from 'lodash';
import $ from 'jquery';

export default function root_init(node) {
  let prods = [];
  ReactDOM.render(<Root products={prods} />, node);
}

class Root extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      products: props.products,
    };

    this.fetch_products();
  }

  fetch_products() {
    $.ajax("/api/v1/products", {
      method: "get",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: "",
      success: (resp) => {
        let state1 = _.assign({}, this.state, { products: resp.data });
        this.setState(state1);
      },
    });
  }

  render() {
    return <div>
      <Header />
      <ProductList products={this.state.products} />
    </div>;
  }
}

function Header(_props) {
  return <div className="row my-2">
    <div className="col-6">
      <h1>Husky Shop</h1>
    </div>
    <div className="col-6">
      <div className="form-inline my-2">
        <input type="email" placeholder="email" />
        <input type="password" placeholder="password" />
        <button className="btn btn-secondary">Login</button>
      </div>
    </div>
  </div>;
}

function ProductList(props) {
  let prods = _.map(props.products, (p) => <Product key={p.id} product={p} />);
  return <div className="row">
    {prods}
  </div>;
}

function Product(props) {
  let {product} = props;
  return <div className="card col-4">
    <div className="card-body">
      <h2 className="card-title">{product.name}</h2>
      <p className="card-text">{product.desc} <br/>
      price: {product.price}</p>
    </div>
  </div>;
}

