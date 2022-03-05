import React, { useEffect } from 'react';
import ReactDOM from 'react-dom';
import Modal from 'react-modal';
let fs = window.require('fs');
// Modal.setAppElement(document.getElementById('App'));

const style = {
    content : {
      top                   : '50%',
      left                  : '50%',
      right                 : 'auto',
      bottom                : 'auto',
      marginRight           : '-50%',
      transform             : 'translate(-50%, -50%)',
      width : '80%',
      height : '80%',
      background: 'red',
      backgroundColor: 'green'
    }
  };
  

function GlobalModal() {

    var subtitle;
    const [modalIsOpen, setIsOpen] = React.useState(false);
    const [modalContents, setContents] = React.useState(undefined);

    useEffect(() => {
      Modal.setAppElement('#App');
      
        window.globalModal = {
            setOpen: setIsOpen,
            setContents: setContents,
        }
        
        return () => {
            // unset the variables
            delete window.globalModal;
        }
    }, []);

    function afterOpenModal() {
        // references are now sync'd and can be accessed.
        // subtitle.style.color = '#f00';
      }
    
      function closeModal(){
        setIsOpen(false);
      }
    
    return (
        <div>
            <Modal
                isOpen={modalIsOpen}
                onAfterOpen={afterOpenModal}
                onRequestClose={closeModal}
                style={style}
                contentLabel="Example Modal"
            >
            <h1>Hello!</h1>
            {modalContents?.()}
            </Modal>
        </div>
    );
}

function Test() {
  return(
    <div>
      <h1>This is a test</h1>
    </div>
  )
}

export default GlobalModal;
