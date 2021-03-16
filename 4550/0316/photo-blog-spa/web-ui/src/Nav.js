import { Nav, Row, Col, Form, Button } from 'react-bootstrap';
import { NavLink } from 'react-router-dom';
import { connect } from 'react-redux';
import { useState } from 'react';

import { api_login } from './api';

function LoginForm() {
  const [name, setName] = useState("");
  const [pass, setPass] = useState("");

  function on_submit(ev) {
    ev.preventDefault();
    api_login(name, pass);
  }

  return (
    <Form onSubmit={on_submit} inline>
      <Form.Control name="name"
                    type="text"
                    onChange={(ev) => setName(ev.target.value)}
                    value={name} />
      <Form.Control name="password"
                    type="password"
                    onChange={(ev) => setPass(ev.target.value)}
                    value={pass} />
      <Button variant="primary" type="submit">
        Login
      </Button>
    </Form>
  );
}

function SessionInfo({session}) {
  return (
    <p>Logged in as {session.name}</p>
  );
}

function LOI({session}) {
  if (session) {
    return <SessionInfo session={session} />;
  }
  else {
    return <LoginForm />;
  }
}

const LoginOrInfo = connect(
  ({session}) => ({session}))(LOI);

function Link({to, children}) {
  return (
    <Nav.Item>
      <NavLink to={to} exact className="nav-link"
               activeClassName="active">
        {children}
      </NavLink>
    </Nav.Item>
  );
}

export default function AppNav() {
  return (
    <Row>
      <Col>
        <Nav variant="pills">
          <Link to="/">Feed</Link>
          <Link to="/users">Users</Link>
        </Nav>
      </Col>
      <Col>
        <LoginOrInfo />
      </Col>
    </Row>
  );
}
