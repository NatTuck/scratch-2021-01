
import { Row, Col, Card } from 'react-bootstrap';
import { connect } from 'react-redux';

function photo_path(post) {
  return "http://localhost:4000/photos/" + post.photo_hash;
}

function Post({post}) {
  return (
    <Col>
      <Card>
        <Card.Img variant="top" src={photo_path(post)} />
        <Card.Text>
          Posted by {post.user.name} <br/>
          {post.body}
        </Card.Text>
      </Card>
    </Col>
  );
}

function Feed({posts}) {
  let cards = posts.map((post) => (
    <Post post={post} key={post.id} />
  ));
  return (
    <div>
      <h2>Feed</h2>
      <Row>{cards}</Row>
    </div>
  );
}

export default connect(({posts}) => ({posts}))(Feed);
