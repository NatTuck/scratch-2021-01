import React from 'react';
import ReactDOM from 'react-dom';
import _ from 'lodash';
import $ from 'jquery';

export default function todo_init(root) {
  ReactDOM.render(<Todo />, root);
}

// App state for Todo is:
//   { items: [List of TodoItem] }
//
// A TodoItem is:
//   { name: String, done: Bool }


class Todo extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      items: [],
    };
  }

  addItem(name) {
    let item = { name: name, done: false };
    let xs = this.state.items.slice(); // clone array
    xs.push(item);
    this.setState({ items: xs });
  }

  addItemClick() {
    let input = $('#add-item-box');
    this.addItem(input.val());
    input.val('');
    console.log(this.state);
  }

  markItem(name) {
    let xs = _.map(this.state.items, (item) => {
      if (item.name == name) {
        return _.extend(item, {done: true});
      }
      else {
        return item;
      }
    });
    this.setState({ items: xs });
  }

  render() {
    let item_list = _.map(this.state.items, (item, ii) => {
      return <TodoItem item={item} markItem={this.markItem.bind(this)} key={ii} />;
    });
    return (
      <div>
        <h2>Tasks:</h2>
        <ul>
          {item_list}
        </ul>
        <h2>Add Item</h2>
        <AddForm add={this.addItemClick.bind(this)} />
      </div>
    );
  }
}

function TodoItem(props) {
  let item = props.item;
  if (item.done) {
    return <li>{item.name} (done)</li>;
  }
  else {
    return <li>{item.name} <button onClick={() => props.markItem(item.name)}>mark</button></li>
  }
}

function AddForm(props) {
  // Allocate new closure on each render.
  // Alternative: Put the function in the root.
  function keyPress(ev) {
    if (ev.key == "Enter") {
      props.add(ev);
    }
  }

  return (
    <div>
      <input type="text" id="add-item-box" onKeyPress={keyPress}/>
      <button onClick={props.add}>Add Item</button>
    </div>
  );
}

