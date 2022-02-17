import React from 'react';
import './Scene.css'
class Scene extends React.Component {
    constructor(props) {
        super(props);
    }

    render() {

        return(
            <div className="Scene">
                < this.props.scene />
            </div>
        );
    }
};

export default Scene;
