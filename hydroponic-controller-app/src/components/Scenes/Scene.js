import React from 'react';

class Scene extends React.Component {
    constructor(props) {
        super(props);
    }

    render() {

        return(
            <div>
                < this.props.scene />
            </div>
        );
    }
};

export default Scene;
